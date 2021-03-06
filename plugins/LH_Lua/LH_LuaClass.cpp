/**
  \file     LH_LuaClass.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <QtDebug>
#include <QDateTime>
#include <QDir>

#include "LH_LuaClass.h"
#include "LH_LuaInstance.h"

QList<LH_LuaClass*> LH_LuaClass::list_;

static void* lh_luaclass_creator( const lh_class *cls )
{
    Q_UNUSED( cls );
    return new LH_LuaInstance( cls );
}

LH_LuaClass::LH_LuaClass( lua_State *luastate, QFileInfo fi, QString filename ) : L(luastate), fi_(fi), filename_(filename)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    memset( &classinfo_, 0, sizeof(classinfo_) );
    classinfo_.size = sizeof(lh_class);

    lua_pushliteral(L,"class_id");
    lua_rawget(L,-2);
    id_array_.append( lua_tostring(L,-1) );
    classinfo_.id = id_array_.constData();
    lua_pop(L,1);

    lua_pushliteral(L,"class_name");
    lua_rawget(L,-2);
    name_array_.append( lua_tostring(L,-1) );
    classinfo_.name = name_array_.constData();
    lua_pop(L,1);

    lua_pushliteral(L,"class_path");
    lua_rawget(L,-2);
    path_array_.append( lua_tostring(L,-1) );
    classinfo_.path = path_array_.constData();
    lua_pop(L,1);

    lua_pushliteral(L,"class_width");
    lua_rawget(L,-2);
    if( lua_isnumber(L,-1) ) classinfo_.width = (int) lua_tointeger(L,-1);
    else classinfo_.width = -1;
    lua_pop(L,1);

    lua_pushliteral(L,"class_height");
    lua_rawget(L,-2);
    if( lua_isnumber(L,-1) ) classinfo_.height = (int) lua_tointeger(L,-1);
    else classinfo_.height = -1;
    lua_pop(L,1);

    // put it into the registry
    lua_pushlightuserdata(L,this);
    lua_pushvalue(L,-2);
    lua_rawset(L,LUA_REGISTRYINDEX);

    // Put the module into lcdhost.class with it's id as the field
    lua_getglobal(L,"lcdhost");
    if( lua_istable(L,-1) )
    {
        lua_pushliteral(L,"class");
        lua_rawget(L,-2);
        if( lua_istable(L,-1) )
        {
            lua_pushlstring(L,id_array_.constData(),id_array_.size());
            lua_pushvalue(L,-4); // copy of module

            // assert the state so I haven't f-ed up
            Q_ASSERT( lua_istable(L,-5) ); // module
            Q_ASSERT( lua_istable(L,-4) ); // lcdhost
            Q_ASSERT( lua_istable(L,-3) ); // lcdhost.class
            Q_ASSERT( lua_isstring(L,-2) ); // module.id
            Q_ASSERT( lua_istable(L,-1) ); // copy of module

            lua_rawset(L,-3); // lcdhost.class.(module.id) = module
        }
        lua_pop(L,1); // pop lcdhost.class
    }
    lua_pop(L,1); // pop lcdhost

    lh_add_class( &classinfo_, lh_luaclass_creator );
    list_.append(this);

    qDebug() << "LH_Lua: loaded" << id_array_.constData() << "from" << filename_;

    Q_ASSERT( old_top == lua_gettop(L) );
    return;
}

LH_LuaClass::~LH_LuaClass()
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    list_.removeAll( this );
    lh_remove_class( &classinfo_ );
    memset( &classinfo_, 0, sizeof(classinfo_) );

    // Unload the module from Lua by setting
    // it's value to nil in lcdhost.class
    lua_getglobal(L,"lcdhost");
    if( lua_istable(L,-1) )
    {
        lua_pushliteral(L,"class");
        lua_rawget(L,-2);
        if( lua_istable(L,-1) )
        {
            lua_pushlstring(L,id_array_.constData(),id_array_.size());
            lua_pushnil(L);
            lua_rawset(L,-3);
        }
        lua_pop(L,1);
    }
    lua_pop(L,1);

    lua_pushlightuserdata(L,this);
    lua_pushnil(L);
    lua_rawset(L,LUA_REGISTRYINDEX);

    Q_ASSERT( old_top == lua_gettop(L) );
    return;
}

void LH_LuaClass::lua_pushmodule()
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    lua_pushlightuserdata(L,this);
    lua_rawget(L,LUA_REGISTRYINDEX);
    Q_ASSERT( lua_istable(L,-1) );
    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return;
}

// This function loads the file and verifies that it
// looks like a valid LCDHost layout class.
// Returns error message or QString() if we have it loaded.
QString LH_LuaClass::load( lua_State *L, QFileInfo fi, const char *datapath )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    QString retv;
    QString filename = fi.fileName();
    QByteArray filename_array = filename.toLocal8Bit();
    const char *magic = NULL;

    if( datapath && *datapath )
    {
        QDir datadir(datapath);
        filename = datadir.relativeFilePath( fi.filePath() );
    }

    Q_ASSERT( L != NULL );

    // Check to see if the file is already loaded,
    // and if so, if it's been changed.
    foreach( LH_LuaClass *alc, list_ )
    {
        if( fi == alc->fi_ )
        {
            // Already loaded.
            if( (fi.lastModified() == alc->fi_.lastModified()) &&
                (fi.size() == alc->fi_.size() ) )
            {
                // Already loaded and not modified, so we're done
                Q_ASSERT( old_top == lua_gettop(L) );
                return retv;
            }

            // It's loaded, but it has been modified, so unload it
            delete alc;
            break;
        }
    }

    // Load the file contents
    QFile file( fi.filePath() );
    QByteArray contents;
    if( file.open( QIODevice::ReadOnly ) )
    {
        contents = file.readAll();
        file.close();
        if( contents.size() != fi.size() )
        {
            // Not ready for this yet, it seems
            retv = QString("%1 size mismatch %2 != %3").arg( filename ).arg(contents.size()).arg(fi.size());
            Q_ASSERT( old_top == lua_gettop(L) );
            goto bail_out;
        }
    }

    // Load the file contents into Lua
    if( luaL_loadbuffer( L, contents.constData(), contents.size(), filename_array.constData() ) )
    {
        // Meh, got an error
        const char *errormsg = lua_tostring(L,-1);
        retv = QString::fromLocal8Bit(errormsg);
        lua_pop(L,1);
        Q_ASSERT( old_top == lua_gettop(L) );
        goto bail_out;
    }

    // Compiled ok, now run it to build the module
    if( lua_pcall(L,0,1,0) )
    {
        // Well, that didn't work
        const char *errormsg = lua_tostring(L,-1);
        retv = QString::fromLocal8Bit(errormsg);
        lua_pop(L,1);
        Q_ASSERT( old_top == lua_gettop(L) );
        goto bail_out;
    }

    // Make sure that got us a table
    if( !lua_istable(L,-1) )
    {
        // this is probably a utility file then
        lua_pop(L,1);
        Q_ASSERT( old_top == lua_gettop(L) );
        return QString();
    }

    // Ensure the table has a 'magic' field
    lua_pushliteral(L,"magic");
    lua_rawget(L,-2);
    if( !lua_isstring(L,-1) )
    {
        retv = QString("%1 doesn't have a magic field").arg(filename);
        lua_pop(L,2);
        Q_ASSERT( old_top == lua_gettop(L) );
        goto bail_out;
    }

    // Check the magic value
    magic = lua_tostring(L,-1);
    if( !magic || strcmp(magic,LUA_MAGIC) )
    {
        retv = QString("%1 has the wrong magic").arg(filename);
        lua_pop(L,2);
        Q_ASSERT( old_top == lua_gettop(L) );
        goto bail_out;
    }
    lua_pop(L,1); // get rid of the magic value, leaving module on top

    // Make sure it has a reasonable id field
    lua_pushliteral(L,"class_id");
    lua_rawget(L,-2);
    if( !lua_isstring(L,-1) )
    {
        retv = QString("%1 doesn't have an id").arg(filename);
        lua_pop(L,2);
        Q_ASSERT( old_top == lua_gettop(L) );
        goto bail_out;
    }
    lua_pop(L,1); // looked good, pop it so the module is at top again

    // Create the class (needs module at stack top)
    new LH_LuaClass(L,fi,filename);
    lua_pop(L,1); // pop module

bail_out:
    Q_ASSERT( old_top == lua_gettop(L) );
    return retv;
}

LH_LuaClass *LH_LuaClass::from_lh_class( const lh_class *cls )
{
    if( cls == NULL ) return NULL;
    foreach( LH_LuaClass *luaclass, list_ )
    {
        if( cls == &luaclass->classinfo_ )
            return luaclass;
    }
    qDebug() << "LH_LuaClass can't find class for" << cls->id;
    return NULL;
}

void LH_LuaClass::clear()
{
#if 0
    while( !list_.isEmpty() )
        delete list_.takeFirst();
#endif
}
