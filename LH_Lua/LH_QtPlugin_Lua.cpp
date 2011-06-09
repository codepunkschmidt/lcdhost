/**
  \file     LH_QtPlugin_Lua.cpp
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


/**
  This plugin looks for .lua and .luac files under DATADIR/lua.
  The search looks inside all the first level subdirectories as well, but no further.
  It will load all lua scripts directly under DATADIR/lua, regardless of their names.
  Inside subdirectories, it will only attempt to load a script if it has the same name
  as the subdirectory, for instance "DATADIR/lua/myclass/myclass.lua".
  */

#include <QtGlobal>

/*
  This is a butt-ugly hack until Qt gets it's act together and exports it's deeply
  hidden CTFontRef using the therefore intended handle() method.
  http://bugreports.qt.nokia.com/browse/QTBUG-17890
  */
#ifdef Q_WS_MAC
#define private public
#include <QFont>
#include "private/qfontengine_p.h"
#undef private
#include <ApplicationServices/ApplicationServices.h>
#include <cairo-quartz.h>
#endif

#include <QDebug>
#include <QDir>
#include <QLibrary>
#include <QCoreApplication>
#include <QFont>
#include <QByteArray>
#include <QColor>
#include <QSettings>
#include <QHash>
#include <QImage>
#include <QPainter>

#include "LH_QtPlugin_Lua.h"
#include "LH_LuaClass.h"
#include "LH_LuaInstance.h"

#include <cairo.h>

int luaopen_xt(lua_State *L); // from luaXT.cpp
extern "C" int luaopen_lcairo(lua_State* L);

LH_PLUGIN(LH_QtPlugin_Lua);
lh_buildinfo buildinfo = LH_STD_BUILDINFO;

#ifndef LUA_STATIC
# include "lua_dyn.c"
lua_All_functions LuaFunctions;
#endif

#ifdef Q_WS_WIN
#include <windows.h>
#include <cairo-win32.h>
#endif

static const lh_systemstate *lcdhost_state()
{
    return LH_LuaInstance::top()->state();
}

static void my_setenv(const char *name, const char *value )
{
#ifdef Q_WS_WIN
    int len = strlen(value)+1+strlen(value)+1;
    char *str = (char*) malloc(len);
    if( str )
    {
        sprintf(str, "%s=%s", name, value);
        putenv(str);
    }
#else
    setenv( name, value, 1 );
#endif
}

extern "C" int lh_lcdhost_rgba(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    QColor c;
    c.setRgba( luaL_checkint(L,1) );
    lua_pushnumber(L,c.redF());
    lua_pushnumber(L,c.greenF());
    lua_pushnumber(L,c.blueF());
    lua_pushnumber(L,c.alphaF());
    Q_ASSERT( old_top+4 == lua_gettop(L) );
    return 4;
}

extern "C" int lh_lcdhost_fontsize(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    double retv = 0.0;

    // extract font size from QFont.toString()
    if( lua_isstring(L,-1) )
    {
        double points, pixels;
        QByteArray str;
        str.append(lua_tostring(L,-1));
        QList<QByteArray> list = str.split(',');
        if( list.size() > 2 )
        {
            points = atof( list.at(1).constData() );
            pixels = atof( list.at(2).constData() );
            if( points > 0 ) retv = points;
            else retv = pixels;
        }
        else
            qDebug() << "lh_lua_fontsize:" << str << "yields" << list.size() << "items";
    }

    if( retv == 0.0 )
    {
        QFont font;
        retv = font.pointSizeF();
    }

    lua_pushnumber(L,retv);
    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return 1;
}

// cs, x, y, fontdata, color, text
extern "C" int lh_lua_draw_text(lua_State *L)
{
    size_t flen = 0;
    size_t slen = 0;

    if( !lua_isuserdata(L,1) ) return 0;

    cairo_surface_t *cs = (cairo_surface_t *) lua_touserdata(L,1);
    int x = luaL_checkinteger(L,2);
    int y = luaL_checkinteger(L,3);
    const char *f = luaL_checklstring(L,4,&flen);
    int c = luaL_checkinteger(L,5);
    const char *s = luaL_checklstring(L,6,&slen);

    QImage img(
            (uchar*) cairo_image_surface_get_data(cs),
            cairo_image_surface_get_width(cs),
            cairo_image_surface_get_height(cs),
            QImage::Format_ARGB32_Premultiplied
            );
    img.setDotsPerMeterX(3780);
    img.setDotsPerMeterY(3780);

    QPainter p;
    if( p.begin(&img) )
    {
        QFont font;

        font.fromString( QString::fromUtf8(f,flen) );
        QFont::StyleStrategy wanted_strategy = ( lcdhost_state()->dev_depth == 1 ) ? QFont::NoAntialias : QFont::PreferAntialias;
        if( !(font.styleStrategy() & wanted_strategy) ) font.setStyleStrategy( wanted_strategy );

        p.setFont( font );
        p.setPen( QColor::fromRgba(c) );
        p.drawText(x,y,QString::fromUtf8(s,slen) );
        p.end();
    }

    return 0;
}

extern "C" int lh_lua_font_face_create(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    QFont font;
    cairo_font_face_t *v = 0;

    if( lua_isstring(L,-1) )
    {
        QString str(lua_tostring(L,-1));
        if( !str.isEmpty() ) font.fromString( str );
    }

#ifdef Q_WS_WIN
    v = cairo_win32_font_face_create_for_hfont ((HFONT)font.handle());
#endif
#ifdef Q_WS_MAC
#ifdef __LP64__
    {
        // Until http://bugreports.qt.nokia.com/browse/QTBUG-17890 gets fixed
        QFontEngine *fe = font.d->engineForScript(QUnicodeTables::Common);
        if (fe && fe->type() == QFontEngine::Mac)
        {
            QCoreTextFontEngine *cte = static_cast<QCoreTextFontEngine*>(fe);
            if( cte ) v = cairo_quartz_font_face_create_for_cgfont( cte->cgFont );
        }
    }
#else
    v = cairo_quartz_font_face_create_for_atsu_font_id ((ATSUFontID)font.macFontID());
#endif
#endif

    lua_pushlightuserdata(L,v);

    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return 1;
}

extern "C" int lh_lua_gettop(lua_State *L)
{
    lua_pushinteger(L,lua_gettop(L));
    return 1;
}

extern "C" int lh_lua_print(lua_State *L)
{
    QByteArray output;
    int n = lua_gettop(L);    /* number of arguments */
    int i;
    for( i = 1; i <= n; i++ )
    {
        // call tostring() on it
        lua_getglobal(L,"tostring");
        lua_pushvalue(L,i);
        lua_pcall(L,1,1,0);
        if( lua_isstring(L, -1) )
            output.append( lua_tostring(L,-1) );
        lua_pop(L,1);
    }
    output.replace( '&', "&amp;" );
    output.replace( '<', "&lt;" );
    output.replace( '>', "&gt;" );
    output.replace( ' ', "&nbsp;" );
    output.replace( '\t', "\\t" );
    QList<QByteArray> lines = output.split('\n');
    foreach( QByteArray line, lines )
    {
        qDebug( "LH_Lua: &nbsp;<tt>%s</tt>", line.constData() );
    }
    Q_ASSERT( lua_gettop(L) == n );
    return 0; // no return values
}

static const char *lh_state_key(int w)
{
    static const char *keys[28] = {
        "dev_width",
        "dev_height",
        "dev_depth",
        "dev_fps",
        "run_time",
        "run_idle",
        "net_max_in",
        "net_max_out",
        "net_cur_in",
        "net_cur_out",
        "net_data_device",
        "net_data_when",
        "net_data_in",
        "net_data_out",
        "mem_data_tot_phys",
        "mem_data_tot_virt",
        "mem_data_free_phys",
        "mem_data_free_virt",
        "cpu_count",
        "cpu_load",
        "cpu_self_now",
        "cpu_self_avg",
        "dir_binaries",
        "dir_plugins",
        "dir_data",
        "dir_layout",
        "layout_file",
        "layout_rps"
    };
    static char core_str[32];
    int core = 1;

    if( w < 1 ) return NULL;
    if( w <= 28 ) return keys[w-1];

    w -= 29;

    if( w >= (lcdhost_state()->cpu_count*3) )
        return NULL;

    while( w>2 )
    {
        ++ core;
        w -= 3;
    }

    switch( w )
    {
    case 0: snprintf(core_str,sizeof(core_str),"cpu_core_system_%d",core); break;
    case 1: snprintf(core_str,sizeof(core_str),"cpu_core_user_%d",core); break;
    case 2: snprintf(core_str,sizeof(core_str),"cpu_core_total_%d",core); break;
    }

    return core_str;
}

static void lh_lcdhost_state_getvalue(lua_State *L,int which)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    if( which <= 28 )
    {
        switch(which)
        {
        case 1: lua_pushinteger( L, lcdhost_state()->dev_width ); break;
        case 2: lua_pushinteger( L, lcdhost_state()->dev_height ); break;
        case 3: lua_pushinteger( L, lcdhost_state()->dev_depth ); break;
        case 4: lua_pushinteger( L, lcdhost_state()->dev_fps ); break;
        case 5: lua_pushinteger( L, lcdhost_state()->run_time ); break;
        case 6: lua_pushinteger( L, lcdhost_state()->run_idle ); break;
        case 7: lua_pushnumber( L, lcdhost_state()->net_max_in ); break;
        case 8: lua_pushnumber( L, lcdhost_state()->net_max_out ); break;
        case 9: lua_pushnumber( L, lcdhost_state()->net_cur_in ); break;
        case 10: lua_pushnumber( L, lcdhost_state()->net_cur_out ); break;
        case 11: lua_pushinteger( L, lcdhost_state()->net_data.device ); break;
        case 12: lua_pushnumber( L, lcdhost_state()->net_data.when ); break;
        case 13: lua_pushnumber( L, lcdhost_state()->net_data.in ); break;
        case 14: lua_pushnumber( L, lcdhost_state()->net_data.out ); break;
        case 15: lua_pushnumber( L, lcdhost_state()->mem_data.tot_phys ); break;
        case 16: lua_pushnumber( L, lcdhost_state()->mem_data.tot_virt ); break;
        case 17: lua_pushnumber( L, lcdhost_state()->mem_data.free_phys ); break;
        case 18: lua_pushnumber( L, lcdhost_state()->mem_data.free_virt ); break;
        case 19: lua_pushinteger( L, lcdhost_state()->cpu_count ); break;
        case 20: lua_pushinteger( L, lcdhost_state()->cpu_load ); break;
        case 21: lua_pushinteger( L, lcdhost_state()->cpu_self_now ); break;
        case 22: lua_pushinteger( L, lcdhost_state()->cpu_self_avg ); break;
        case 23: lua_pushstring( L, lcdhost_state()->dir_binaries ); break;
        case 24: lua_pushstring( L, lcdhost_state()->dir_plugins ); break;
        case 25: lua_pushstring( L, lcdhost_state()->dir_data ); break;
        case 26: lua_pushstring( L, lcdhost_state()->dir_layout ); break;
        case 27: lua_pushstring( L, lcdhost_state()->layout_file ); break;
        case 28: lua_pushinteger( L, lcdhost_state()->layout_rps ); break;
        default: lua_pushnil(L); break;
        }
    }
    else
    {
        int core = 0;
        which -= 29;

        if( which < (lcdhost_state()->cpu_count*3) )
        {
            while( which>2 )
            {
                ++ core;
                which -= 3;
            }

            switch( which )
            {
            case 0:
                lua_pushnumber(L,lcdhost_state()->cpu_cores[core].system);
                break;
            case 1:
                lua_pushnumber(L,lcdhost_state()->cpu_cores[core].user);
                break;
            case 2:
                lua_pushnumber(L,lcdhost_state()->cpu_cores[core].total);
                break;
            default:
                Q_ASSERT(0);
                lua_pushnil(L);
                break;
            }
        }
        else
            lua_pushnil(L);
    }

    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return;
}

extern "C" int lh_lcdhost_state_index(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    if( !lcdhost_state() ) return 0;

    if( lua_isnumber(L,-1) )
    {
        lh_lcdhost_state_getvalue(L,lua_tonumber(L,-1));
    }
    else if( lua_isstring(L,-1) )
    {
        const char *idx = lua_tostring(L,-1);
        int w = 0;
        for( w=1; w>=1; ++w )
        {
            const char *k = lh_state_key(w);
            if( k == NULL ) { w = 0; break; }
            if( !strcmp( k, idx ) ) break;
        }
        lh_lcdhost_state_getvalue(L,w);
    }
    else
        lua_pushnil(L);

    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return 1;
}

extern "C" int lh_block_newindex(lua_State *L)
{
    Q_UNUSED(L);
    return 0;
}

extern "C" int lh_lcdhost_state_next(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    const char *lsk = NULL;
    int where = 0;

    if( lua_isstring(L,2) )
    {
        const char *k = lua_tostring(L,2);
        for( int i=1;!where;++i)
        {
            lsk = lh_state_key(i);
            if( lsk == NULL ) break;
            if( !strcmp(k,lsk) ) where = i;
        }
    }

    if( lua_isnumber(L,2) )
        where = (int) lua_tonumber(L,2);

    if( lua_isnil(L,2) )
        where = 0;

    ++ where;
    lsk = lh_state_key(where);
    if( lsk )
    {
        lua_pushstring( L, lsk );
        lh_lcdhost_state_index( L );
        Q_ASSERT( old_top+2 == lua_gettop(L) );
        return 2;
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_lcdhost_state_pairs(lua_State *L)
{
    lua_pushcclosure( L, lh_lcdhost_state_next, 0 );
    lua_newtable( L );
    lua_pushnil( L );
    return 3;
}

const char *LH_QtPlugin_Lua::lh_load()
{
    QString lua_cpath;
    QString lua_path;
    QString path;
    const char *env_lua_cpath;
    const char *env_lua_path;
    const char *env_path;
    const char *dir_data_p = 0;
    const char *dir_plugins_p = 0;

    callback( lh_cb_dir_data, &dir_data_p );
    callback( lh_cb_dir_plugins, &dir_plugins_p );

    QString dir_data = QString::fromUtf8( dir_data_p ? dir_data_p : "" );
    QString dir_plugins = QString::fromUtf8( dir_plugins_p ? dir_plugins_p : "" );

    // Create DATADIR/lua/clibs if it doesn't exist
    {
        QDir datadir( dir_data );
        if( !datadir.exists(LUADIR "/clibs") ) datadir.mkpath(LUADIR "/clibs");
    }

    // Add our paths to the environment if they're not already there
    env_lua_cpath = getenv("LUA_CPATH");
    if( !QString::fromLocal8Bit(env_lua_cpath).contains( dir_data ) &&
        !QString::fromLocal8Bit(env_lua_cpath).contains( dir_plugins )
        )
    {
        lua_cpath.append( dir_data );
#ifdef Q_OS_MAC
        lua_cpath.append( "lua/clibs/?.so;" );
#endif
#ifdef Q_OS_WIN
        lua_cpath.append( "lua/clibs/?.dll;" );
#endif
        lua_cpath.append( dir_plugins );
#ifdef Q_OS_MAC
        lua_cpath.append( "?.so" );
#endif
#ifdef Q_OS_WIN
        lua_cpath.append( "?.dll" );
#endif
        if( env_lua_cpath && *env_lua_cpath )
        {
            lua_cpath.append( ';' );
            lua_cpath.append( QString::fromLocal8Bit(env_lua_cpath) );
        }
    }
    env_lua_path = getenv("LUA_PATH");
    if( !QString::fromLocal8Bit(env_lua_path).contains( dir_data) )
    {
        lua_path.append( dir_data );
        lua_path.append( "lua/?.lua;" );
        lua_path.append( dir_data );
        lua_path.append( "lua/?.luac;" );
        lua_path.append( dir_data );
        lua_path.append( "lua/?/init.lua" );
        if( env_lua_path && *env_lua_path )
        {
            lua_path.append( ';' );
            lua_path.append( QString::fromLocal8Bit(env_lua_path) );
        }
    }

    env_path = getenv("PATH");
    if( !QString::fromLocal8Bit(env_path).contains( dir_data) )
    {
        if( env_path && *env_path )
        {
            path.append( QString::fromLocal8Bit(env_path) );
#ifdef Q_OS_WIN
            path.append( ';' );
#else
            path.append( ':' );
#endif
        }
        path.append( dir_data );
        path.append( "lua/clibs" );
    }

#ifdef Q_OS_WIN
    // Lua for Windows workarounds.
    //  It doesn't set LUA_CPATH
    //  It limits it's LUA_PATH to *.luac
    QByteArray lua_dev;
    lua_dev.append( getenv("LUA_DEV") );
    if( !lua_dev.isEmpty() )
    {
        if( !lua_cpath.isEmpty() )
        {
            lua_cpath.append( ';' );
            lua_cpath.append( lua_dev );
            lua_cpath.append( "\\clibs\\?.dll" );
        }
        if( !lua_path.isEmpty() )
        {
            lua_path.append( ';' );
            lua_path.append( lua_dev );
            lua_path.append( "\\lua\\?.lua;" );
            lua_path.append( lua_dev );
            lua_path.append( "\\lua\\?\\init.lua" );
        }
    }
#endif

    // Set the new environment variables
    {
        QByteArray ary;
        if( !lua_cpath.isEmpty() )
        {
            ary = QDir::toNativeSeparators(lua_cpath).toLocal8Bit();
            my_setenv( "LUA_CPATH", ary.data() );
        }
        if( !lua_path.isEmpty() )
        {
            ary = QDir::toNativeSeparators(lua_path).toLocal8Bit();
            my_setenv( "LUA_PATH", ary.data() );
        }
        if( !path.isEmpty() )
        {
            ary = QDir::toNativeSeparators(path).toLocal8Bit();
            my_setenv( "PATH", ary.data() );
        }
    }

    // Start Lua
    L = luaL_newstate();
    if( L == NULL ) return "luaL_newstate() failed";
    luaL_openlibs(L);

    // init luaXT for the __pairs metamethod
    luaopen_xt(L);

    // load gd
    // luaopen_gd(L);

    luaopen_lcairo(L);

#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    // extend cairo with font_face_create and draw_text
    lua_getglobal(L,"cairo");
    lua_pushliteral(L,"font_face_create");
    lua_pushcclosure(L,lh_lua_font_face_create,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"draw_text");
    lua_pushcclosure(L,lh_lua_draw_text,0);
    lua_rawset(L,-3);
    lua_pop(L,1);

    // Create the lcdhost global variable
    // and it's fields 'state', 'class' and 'instance'
    lua_newtable(L);
    lua_pushliteral(L,"state");
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushliteral(L,"class");
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushliteral(L,"instance");
    lua_newtable(L);
    lua_rawset(L,-3);
    lua_pushliteral(L,"gettop");
    lua_pushcclosure(L,lh_lua_gettop,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"fontsize");
    lua_pushcclosure(L,lh_lcdhost_fontsize,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"rgba");
    lua_pushcclosure(L,lh_lcdhost_rgba,0);
    lua_rawset(L,-3);
    lua_setglobal(L, "lcdhost");

    // set the lcdhost.state metatable
    lua_getglobal(L,"lcdhost");
    lua_pushliteral(L,"state");
    lua_rawget(L,-2);
    lua_newtable(L);
    lua_pushliteral(L,"__index");
    lua_pushcclosure(L,lh_lcdhost_state_index,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"__newindex");
    lua_pushcclosure(L,lh_block_newindex,0);
    lua_rawset(L,-3);
    lua_pushliteral(L,"__pairs");
    lua_pushcclosure(L,lh_lcdhost_state_pairs,0);
    lua_rawset(L,-3);
    lua_setmetatable(L,-2);
    lua_pop(L,2);

    // redirect print()
    lua_pushliteral(L,"print");
    lua_pushcclosure(L,lh_lua_print,0);
    lua_rawset(L,LUA_GLOBALSINDEX);

    // report on Lua version and source
    lua_getglobal(L,"_VERSION");
    if( lua_isstring(L,-1) )
    {
        qDebug() << "LH_Lua: running" << lua_tostring(L,-1);
    }
    lua_pop(L,1);

    Q_ASSERT( old_top == lua_gettop(L) );

    // make the font -> filename map
    // lh_make_fontmap();

    luadir_ = dir_data.append("lua");
    // watcher_ = NULL;
    // watcher_ = new QFileSystemWatcher();
    // connect( watcher_, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString))  );
    // watcher_->addPath( luadir_);
    directoryChanged(luadir_);

    return NULL;
}

void LH_QtPlugin_Lua::lh_unload()
{
    // if( watcher_ ) { delete watcher_; watcher_ = NULL; }
    LH_LuaClass::clear();
    if( L ) { lua_close(L); L = NULL; }
    // if( lh_fontmap ) { delete lh_fontmap; lh_fontmap = NULL; }
}

int LH_QtPlugin_Lua::notify( int code, void *)
{
    if( !code || code & LH_NOTE_SECOND )
    {
        if( needscan_ && dirmodified_.elapsed() > 1000 )
        {
            scanForFiles();
        }
    }
    return LH_NOTE_SECOND;
}

void LH_QtPlugin_Lua::loadLuaFile( QFileInfo fi )
{
    const char *dir_data_p = 0;
    callback( lh_cb_dir_data, &dir_data_p );
    QString err = LH_LuaClass::load(L,fi,dir_data_p ? dir_data_p : "");
    if( !err.isEmpty() ) qWarning() << "LH_Lua:" << err;
    return;
}

void LH_QtPlugin_Lua::directoryChanged(QString)
{
    dirmodified_.start();
    needscan_ = true;
}

void LH_QtPlugin_Lua::scanForFiles()
{
    QDir dir(luadir_);

    needscan_ = false;
    dir.setFilter( QDir::Files | QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot );
    foreach( QFileInfo fi, dir.entryInfoList() )
    {
        if( fi.isFile() && (fi.suffix() == "lua" || fi.suffix() == "luac") )
        {
            loadLuaFile(fi);
        }
        else if( fi.isDir() )
        {
            QDir subdir(fi.filePath());
            subdir.setFilter( QDir::Files | QDir::Readable );
            foreach( QFileInfo sfi, subdir.entryInfoList() )
            {
                if( sfi.baseName() == fi.baseName() && (sfi.suffix() == "lua" || sfi.suffix() == "luac") )
                {
                    loadLuaFile(sfi);
                }
            }
        }
    }

    return;
}
