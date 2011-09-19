/**
  \file     LH_LuaInstance.cpp
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
#include <QColor>

#include "LH_LuaInstance.h"
#include "LH_LuaClass.h"

#include <cairo.h>

extern "C" int lh_lua_print(lua_State *L);

QStack<LH_LuaInstance*> *LH_LuaInstance::stack_ = 0;

static const char *lh_self_item_data_key[] =
{
    NULL,
    "name", "type", "data", "help", "flags", "list", "min", "max", "order"
};

#define MAXWHERE ((int)(sizeof(lh_self_item_data_key)/sizeof(const char *))-1)

// Closure data: self
// Params: string
extern "C" int lh_lcdhost_callback(lua_State *L)
{
    LH_LuaInstance *inst = NULL;
    if( lua_istable(L,lua_upvalueindex(1)) ) // self
    {
        if( lua_isstring(L,1) ) // callback name
        {
            const char *s = lua_tostring(L,1);
            lua_getfield( L, lua_upvalueindex(1), "token" );
            inst = (LH_LuaInstance *) lua_touserdata(L,2);
            if( inst && s && *s )
            {
                if( !strcmp(s,"polling") )
                {
                    inst->callback( lh_cb_polling, 0 );
                }
                if( !strcmp(s,"notify") )
                {
                    inst->callback( lh_cb_notify, 0 );
                }
                if( !strcmp(s,"render") )
                {
                    inst->callback( lh_cb_render, 0 );
                }
            }
            lua_pop(L,1);
        }
    }
    return 0;
}

// expect type string at stack top
static void lh_settype_setup_item( lua_State *L, lua_setup_item *item )
{
    const char *t = lua_tostring(L,-1);
    item->item()->type = lh_type_none;
    switch( *t )
    {
    case 'i': // integer, inputstate, inputvalue, image
        if( !strcmp(t,"integer") ) item->item()->type = lh_type_integer;
        else if( !strcmp(t,"inputstate") ) item->item()->type = lh_type_string_inputstate;
        else if( !strcmp(t,"inputvalue") ) item->item()->type = lh_type_string_inputvalue;
        else if( !strcmp(t,"image") ) item->item()->type = lh_type_image_png;
        break;
    case 'f': // fraction, font, filename
        if( !strcmp(t,"font") ) item->item()->type = lh_type_string_font;
        else if( !strcmp(t,"fraction") ) item->item()->type = lh_type_fraction;
        else if( !strcmp(t,"filename") ) item->item()->type = lh_type_string_filename;
        break;
    case 'c': // color
        if( !strcmp(t,"color") )
        {
            item->item()->type = lh_type_integer_color;
            item->item()->data.i = QColor(Qt::black).rgba();
        }
        break;
    case 's': // slider, string, script
        if( !strcmp(t,"string") ) item->item()->type = lh_type_string;
        else if( !strcmp(t,"slider") ) item->item()->type = lh_type_integer_slider;
        else if( !strcmp(t,"script") ) item->item()->type = lh_type_string_script;
        break;
    case 'p': // progress
        if( !strcmp(t,"progress") ) item->item()->type = lh_type_integer_progress;
        break;
    case 'l': // list
        if( !strcmp(t,"list") ) item->item()->type = lh_type_integer_list;
        if( !strcmp(t,"listbox") ) item->item()->type = lh_type_integer_listbox;
        break;
    case 'b':
        if( !strcmp(t,"boolean") ) item->item()->type = lh_type_integer_boolean;
        if( !strcmp(t,"button") ) item->item()->type = lh_type_string_button;
        break;
    }
    if( item->item()->type == lh_type_none )
    {
        lua_pushstring(L,"setup item type unrecognized");
        lua_error(L);
    }
    memset( & item->item()->param, 0, sizeof(lh_setup_param) );
}


static void lh_self_item_data_setvalue(lua_State *L, lua_setup_item* item, int where)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    Q_ASSERT( item != NULL );
    Q_ASSERT( where >= 1 );
    Q_ASSERT( where <= MAXWHERE );

    switch( where )
    {
    case 1:
        Q_ASSERT( !strcmp("name",lh_self_item_data_key[where]));
        item->name().clear();
        item->name().append( lua_tostring(L,-1) );
        item->item()->name = item->name().constData();
        break;
    case 2:
        Q_ASSERT( !strcmp("type",lh_self_item_data_key[where]));
        lh_settype_setup_item( L, item );
        break;
    case 3:
        Q_ASSERT( !strcmp("data",lh_self_item_data_key[where]));
        switch( item->item()->type )
        {
        case lh_type_none:
        case lh_type_last:
            lua_pushstring(L,"setup item has no 'type' set");
            lua_error(L);
            break;
        case lh_type_integer:
        case lh_type_integer_slider:
        case lh_type_integer_progress:
            {
                int i = lua_tointeger(L,-1);
                if( item->item()->param.slider.min != item->item()->param.slider.max )
                {
                    if( i < item->item()->param.slider.min ) i = item->item()->param.slider.min;
                    if( i > item->item()->param.slider.max ) i = item->item()->param.slider.max;
                }
                item->item()->data.i = i;
            }
            break;
        case lh_type_integer_color:
            if( lua_isnumber(L,-1) )
                item->item()->data.i = lua_tointeger(L,-1);
            else if( lua_isstring(L,-1) )
            {
                QColor col( lua_tostring(L,-1) );
                item->item()->data.i = col.rgba();
            }
            else
                item->item()->data.i = QColor(Qt::black).rgba();
            break;
        case lh_type_integer_list:
        case lh_type_integer_listbox:
            // in Lua, lists start at 1!
            item->item()->data.i = lua_tointeger(L,-1) - 1;
            break;
        case lh_type_integer_boolean:
            item->item()->data.i = (int) lua_toboolean(L,-1);
            break;
        case lh_type_fraction:
            {
                float f = (float) lua_tonumber(L,-1);
                if( item->item()->param.range.min != item->item()->param.range.max )
                {
                    if( f < item->item()->param.range.min ) f = item->item()->param.range.min;
                    if( f > item->item()->param.range.max ) f = item->item()->param.range.max;
                }
                item->item()->data.f = f;
            }
            break;
        case lh_type_string:
        case lh_type_string_htmlhint:
        case lh_type_string_button:
        case lh_type_string_script:
        case lh_type_string_filename:
        case lh_type_string_font:
        case lh_type_string_inputstate:
        case lh_type_string_inputvalue:
        case lh_type_image_png:
        case lh_type_image_qimage:
            {
                size_t len = 0;
                const char *str = lua_tolstring( L, -1, &len );
                item->data().clear();
                item->data().append( str, (int) len );
                item->item()->data.s = item->data().data();
                item->item()->param.size = item->data().size();
            }
            break;
        }
        break;
    case 4:
        Q_ASSERT( !strcmp("help",lh_self_item_data_key[where]));
        item->help().clear();
        item->help().append( lua_tostring(L,-1) );
        item->item()->help = item->help().constData();
        break;
    case 5:
        Q_ASSERT( !strcmp("flags",lh_self_item_data_key[where]));
        if( lua_isstring(L,-1) )
        {
            const char *str = lua_tostring(L,-1);
            int len = strlen(str);
            item->item()->flags = 0;
            for( int i=0; i<len; i++ )
            {
                switch( str[i] )
                {
                case 'r': item->item()->flags |= LH_FLAG_READONLY; break;
                case 'h': item->item()->flags |= LH_FLAG_HIDDEN; break;
                case 'f': item->item()->flags |= LH_FLAG_FOCUS; break;
                case 'a': item->item()->flags |= LH_FLAG_AUTORENDER; break;
                case '<':
                    item->item()->flags |= LH_FLAG_FIRST;
                    item->setOrder(-1);
                    break;
                case '>':
                    item->item()->flags |= LH_FLAG_LAST;
                    item->setOrder(1);
                    break;
                }
            }
        }
        break;
    case 6:
        Q_ASSERT( !strcmp("list",lh_self_item_data_key[where]) );
        if( item->item()->type != lh_type_integer_list &&
            item->item()->type != lh_type_integer_listbox )
        {
            lua_pushstring(L,"must set 'type' to \"list\" or \"listbox\" before setting 'list'");
            lua_error(L);
            break;
        }

        if( lua_isstring(L,-1) )
        {
            size_t len = 0;
            const char *str = lua_tolstring(L,-1,&len);
            item->paramlist().clear();
            item->paramlist().append(str,(int)len);
            item->item()->param.list = item->paramlist().constData();
            break;
        }

        if( lua_istable(L,-1) )
        {
            item->paramlist().clear();
            lua_pushnil(L);
            while(lua_next(L, -2))
            {
                size_t len = 0;
                const char *str = lua_tolstring(L,-1,&len);
                QByteArray ary(str,(int)len);
                ary.replace('\t',' ');
                if( !item->paramlist().isEmpty() ) item->paramlist().append('\t');
                item->paramlist().append(ary);
                lua_pop(L, 1);
            }
            item->item()->param.list = item->paramlist().constData();
        }
        break;
    case 7:
        Q_ASSERT( !strcmp("min",lh_self_item_data_key[where]));
        if( item->item()->type == lh_type_integer ||
            item->item()->type == lh_type_integer_slider ||
            item->item()->type == lh_type_integer_progress )
        {
            item->item()->param.slider.min = lua_tointeger(L,-1);
        }
        else if( item->item()->type == lh_type_fraction )
        {
            item->item()->param.range.min = lua_tonumber(L,-1);
        }
        else
        {
            lua_pushstring(L,"must set 'type' to \"int\", \"slider\", \"progress\" or \"fraction\" before setting 'min'");
            lua_error(L);
        }
        break;
    case 8:
        Q_ASSERT( !strcmp("max",lh_self_item_data_key[where]));
        if( item->item()->type == lh_type_integer ||
            item->item()->type == lh_type_integer_slider ||
            item->item()->type == lh_type_integer_progress )
        {
            item->item()->param.slider.max = lua_tointeger(L,-1);
        }
        else if( item->item()->type == lh_type_fraction )
        {
            item->item()->param.range.max = lua_tonumber(L,-1);
        }
        else
        {
            lua_pushstring(L,"must set 'type' to \"int\", \"slider\", \"progress\" or \"fraction\" before setting 'max'");
            lua_error(L);
        }
        break;
    case 9:
        Q_ASSERT( !strcmp("order",lh_self_item_data_key[where]));
        item->setOrder( (int) lua_tonumber(L,-1) );
        break;
    }

    // refresh the setup item
    item->parent()->callback(lh_cb_setup_refresh,item->item());
    Q_ASSERT( old_top == lua_gettop(L) );
}

static void lh_self_item_data_getvalue(lua_State *L, lua_setup_item* item, int where)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    Q_ASSERT( item != NULL );
    Q_ASSERT( where >= 1 );
    Q_ASSERT( where <= MAXWHERE );

    switch( where )
    {
    case 1:
        Q_ASSERT( !strcmp("name",lh_self_item_data_key[where]));
        lua_pushstring(L,item->item()->name );
        break;
    case 2:
        Q_ASSERT( !strcmp("type",lh_self_item_data_key[where]));
        switch( item->item()->type )
        {
        case lh_type_none:
        case lh_type_last:
            lua_pushstring(L,"invalid");
            break;
        case lh_type_integer:
            lua_pushstring(L,"integer");
            break;
        case lh_type_fraction:
            lua_pushstring(L,"fraction");
            break;
        case lh_type_integer_color:
            lua_pushstring(L,"number");
            break;
        case lh_type_integer_slider:
            lua_pushstring(L,"slider");
            break;
        case lh_type_integer_progress:
            lua_pushstring(L,"progress");
            break;
        case lh_type_integer_list:
            lua_pushstring(L,"list");
            break;
        case lh_type_integer_listbox:
            lua_pushstring(L,"listbox");
            break;
        case lh_type_integer_boolean:
            lua_pushstring(L,"boolean");
            break;
        case lh_type_string:
        case lh_type_string_htmlhint:
        case lh_type_string_script:
            lua_pushstring(L,"string");
            break;
        case lh_type_string_button:
            lua_pushstring(L,"button");
            break;
        case lh_type_string_filename:
            lua_pushstring(L,"filename");
            break;
        case lh_type_string_font:
            lua_pushstring(L,"font");
            break;
        case lh_type_string_inputstate:
            lua_pushstring(L,"inputstate");
            break;
        case lh_type_string_inputvalue:
            lua_pushstring(L,"inputvalue");
            break;
        case lh_type_image_png:
        case lh_type_image_qimage:
            lua_pushstring(L,"image");
            break;
        }
        break;
    case 3:
        Q_ASSERT( !strcmp("data",lh_self_item_data_key[where]));
        switch( item->item()->type )
        {
        case lh_type_none:
        case lh_type_last:
            lua_pushnil(L);
            break;
        case lh_type_integer:
        case lh_type_integer_color:
        case lh_type_integer_slider:
        case lh_type_integer_progress:
            lua_pushinteger(L,item->item()->data.i);
            break;
        case lh_type_integer_list:
        case lh_type_integer_listbox:
            // in Lua, lists start at 1!
            lua_pushinteger(L,item->item()->data.i + 1);
            break;
        case lh_type_integer_boolean:
            lua_pushboolean(L,item->item()->data.i);
            break;
        case lh_type_fraction:
            lua_pushnumber(L,item->item()->data.f);
            break;
        case lh_type_string:
        case lh_type_string_htmlhint:
        case lh_type_string_button:
        case lh_type_string_script:
        case lh_type_string_font:
        case lh_type_string_filename:
        case lh_type_string_inputstate:
        case lh_type_string_inputvalue:
            lua_pushstring(L,item->item()->data.s);
            break;
        case lh_type_image_png:
        case lh_type_image_qimage:
            lua_pushlstring(L,item->item()->data.s,item->item()->param.size);
            break;
        }
        break;
    case 4: // help
        Q_ASSERT( !strcmp("help",lh_self_item_data_key[where]));
        lua_pushstring(L,item->item()->help );
        break;
    case 5: // flags
        Q_ASSERT( !strcmp("flags",lh_self_item_data_key[where]));
        if( item->item()->flags )
        {
            QByteArray ary;
            if( item->item()->flags & LH_FLAG_READONLY ) ary.append('r');
            if( item->item()->flags & LH_FLAG_HIDDEN ) ary.append('h');
            if( item->item()->flags & LH_FLAG_FOCUS ) ary.append('f');
            if( item->item()->flags & LH_FLAG_AUTORENDER ) ary.append('a');
            if( item->item()->flags & LH_FLAG_FIRST || item->order() == -1 ) ary.append('<');
            if( item->item()->flags & LH_FLAG_LAST || item->order() == 1 ) ary.append('>');
            lua_pushstring(L,ary.constData());
        }
        else
            lua_pushnil(L);
        break;
    case 6: // list
        Q_ASSERT( !strcmp("list",lh_self_item_data_key[where]));
        if( (item->item()->type == lh_type_integer_list || item->item()->type == lh_type_integer_listbox)
            && item->item()->param.list )
            lua_pushstring(L,item->item()->param.list);
        else
            lua_pushnil(L);
        break;
    case 7: // min
        Q_ASSERT( !strcmp("min",lh_self_item_data_key[where]));
        if( item->item()->type == lh_type_integer ||
            item->item()->type == lh_type_integer_slider ||
            item->item()->type == lh_type_integer_progress )
            lua_pushnumber(L,item->item()->param.slider.min);
        else
            lua_pushnil(L);
        break;
    case 8: // max
        Q_ASSERT( !strcmp("max",lh_self_item_data_key[where]));
        if( item->item()->type == lh_type_integer ||
            item->item()->type == lh_type_integer_slider ||
            item->item()->type == lh_type_integer_progress )
            lua_pushnumber(L,item->item()->param.slider.max);
        else
            lua_pushnil(L);
        break;
    case 9: // order
        Q_ASSERT( !strcmp("order",lh_self_item_data_key[where]));
        if( item->order() )
            lua_pushnumber(L,item->order());
        else
            lua_pushnil(L);
        break;
    default:
        lua_pushnil(L);
        break;
    }

    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return;
}

extern "C" int lh_self_item_data_index(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    lua_setup_item* item = (lua_setup_item*) lua_touserdata(L,lua_upvalueindex(1));
    if( item )
    {
        int where = 0;

        if( lua_isstring(L,-1) )
        {
            const char *k = lua_tostring(L,-1);
            for( int i=1; i<=MAXWHERE; ++i )
            {
                if( !strcmp(k,lh_self_item_data_key[i]) )
                {
                    where = i;
                    break;
                }
            }
        }

        if( lua_isnumber(L,-1) )
            where = (int) lua_tonumber(L,-1);

        lh_self_item_data_getvalue(L,item,where);
        Q_ASSERT( old_top+1 == lua_gettop(L) );
        return 1;
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_self_item_data_newindex(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    lua_setup_item* item = (lua_setup_item*) lua_touserdata(L,lua_upvalueindex(1));
    if( item )
    {
        int where = 0;

        if( lua_isstring(L,-2) )
        {
            const char *k = lua_tostring(L,-2);
            for( int i=1; i<=MAXWHERE; ++i )
            {
                if( !strcmp(k,lh_self_item_data_key[i]) )
                {
                    where = i;
                    break;
                }
            }
        }

        if( lua_isnumber(L,-2) )
            where = (int) lua_tonumber(L,-2);

        lh_self_item_data_setvalue(L,item,where);
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_self_item_data_next(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    lua_setup_item* item = (lua_setup_item*) lua_touserdata(L,lua_upvalueindex(1));

    if( item )
    {
        int where = 0;

        if( lua_isstring(L,2) )
        {
            const char *k = lua_tostring(L,2);
            for( int i=1; i<=MAXWHERE; ++i )
            {
                if( !strcmp(k,lh_self_item_data_key[i]) )
                {
                    where = i;
                    break;
                }
            }
        }

        if( lua_isnumber(L,2) )
            where = (int) lua_tonumber(L,2);

        if( lua_isnil(L,2) )
            where = 0;

        while( where < MAXWHERE )
        {
            ++ where;
            lua_pushstring( L, lh_self_item_data_key[where] );
            lh_self_item_data_getvalue(L,item,where);
            if( !lua_isnil(L,-1) )
            {
                Q_ASSERT( old_top+2 == lua_gettop(L) );
                return 2;
            }
            lua_pop(L,2);
        }
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_self_item_data_pairs(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    lua_setup_item* item = (lua_setup_item*) lua_touserdata(L,lua_upvalueindex(1));

    if( item )
    {
        lua_pushlightuserdata( L, item );
        lua_pushcclosure( L, lh_self_item_data_next, 1 );
        lua_newtable( L );
        lua_pushnil( L );
        Q_ASSERT( old_top+3 == lua_gettop(L) );
        return 3;
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

// push a table with metatable for the setup item
static void lh_push_setup_item( lua_State *L, lua_setup_item* item )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    lua_newtable(L);
    lua_newtable(L); // the metatable
    lua_pushliteral(L,"__index"); // 4
    lua_pushlightuserdata(L,item); // 5
    lua_pushcclosure(L,lh_self_item_data_index,1); // 5
    lua_rawset(L,-3); // 3
    lua_pushliteral(L,"__newindex"); // 4
    lua_pushlightuserdata(L,item); // 5
    lua_pushcclosure(L,lh_self_item_data_newindex,1); // 5
    lua_rawset(L,-3); // 3
    lua_pushliteral(L,"__pairs"); // 4
    lua_pushlightuserdata(L,item); // 5
    lua_pushcclosure(L,lh_self_item_data_pairs,1); // 5
    lua_rawset(L,-3); // 3
    lua_setmetatable(L,-2); // 2
    Q_ASSERT( old_top+1 == lua_gettop(L) );
    return;
}

// expects table at stack top
static void lh_populate_setup_item_from_table( lua_State *L, lua_setup_item *item )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    Q_ASSERT( lua_istable(L,-1) );

    for( int where=1; where<=MAXWHERE; ++where )
    {
        lua_pushstring(L,lh_self_item_data_key[where]);
        lua_rawget(L,-2);
        if( !lua_isnil(L,-1) )
            lh_self_item_data_setvalue(L,item,where);
        lua_pop(L,1);
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return;
}

// given the 'self' table at that index, get it's
// token and cast to LH_LuaInstance
static LH_LuaInstance *lh_get_self(lua_State *L, int index)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    LH_LuaInstance *self = NULL;
    Q_ASSERT( lua_istable(L,index) );
    lua_pushvalue(L,index);
    lua_pushliteral(L,"token");
    lua_rawget(L,-2);
    self = reinterpret_cast<LH_LuaInstance *>(lua_touserdata(L,-1));
    lua_pop(L,2);
    Q_ASSERT( old_top == lua_gettop(L) );
    return self;
}

extern "C" int lh_self_item_index(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    LH_LuaInstance *self = lh_get_self(L,lua_upvalueindex(1));

    if( self )
    {
        lua_setup_item *item;
        int where = -1;

        if( lua_isnumber(L,2) )
            where = (int) lua_tonumber(L,2);

        if( lua_isstring(L,2) )
        {
            const char *k = lua_tostring(L,2);
            int i = 0;
            foreach( item, self->items() )
            {
                if( !strcmp( item->key(), k )  ) { where = i; break; }
                ++ i;
            }
        }

        if( where >= 0 && where < self->items().size() )
        {
            lh_push_setup_item( L, self->items().at(where) );
            Q_ASSERT( old_top+1 == lua_gettop(L) );
            return 1;
        }
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_self_item_newindex(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    LH_LuaInstance *self = lh_get_self(L,lua_upvalueindex(1));

    if( self )
    {
        lua_setup_item *item;
        int where = -1;

        if( lua_isnumber(L,2) )
            where = (int) lua_tonumber(L,2);

        if( lua_isstring(L,2) )
        {
            const char *k = lua_tostring(L,2);
            int i = 0;
            foreach( item, self->items() )
            {
                if( !strcmp( item->key(), k ) ) { where = i; break; }
                ++ i;
            }
        }

        if( where >= 0 && where < self->items().size() )
        {
            lua_pushvalue(L,3);
            lh_populate_setup_item_from_table( L, self->items().at(where) );
            Q_ASSERT( old_top == lua_gettop(L) );
            return 0;
        }

        if( lua_isstring(L,2) && lua_istable(L,3) )
        {
            item = new lua_setup_item(self,lua_tostring(L,2));
            lua_pushvalue(L,3);
            lh_populate_setup_item_from_table( L, item );
            self->items().append( item );
            lua_pop(L,1);
            Q_ASSERT( old_top == lua_gettop(L) );
            return 0;
        }

        // rebuild list of setup items
        self->requestRebuild();
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_self_item_next(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    LH_LuaInstance *self = lh_get_self(L,lua_upvalueindex(1));

    if( self )
    {
        lua_setup_item *item;
        int where = -2;

        if( lua_isnumber(L,2) )
            where = (int) lua_tonumber(L,2);

        if( lua_isstring(L,2) )
        {
            int i = 0;
            foreach( item, self->items() )
            {
                if( !strcmp( item->key(), lua_tostring(L,2) ) )
                {
                    where = i;
                    break;
                }
                ++ i;
            }
        }

        if( lua_isnil(L,2) )
            where = -1;

        ++ where;

        if( where < 0 || where >= self->items().size() )
        {
            lua_pushnil(L);
            Q_ASSERT( old_top+1 == lua_gettop(L) );
            return 1;
        }

        lua_pushstring( L, self->items().at(where)->key() );
        lh_push_setup_item( L, self->items().at(where) );
        Q_ASSERT( old_top+2 == lua_gettop(L) );
        return 2;
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

extern "C" int lh_self_item_pairs(lua_State *L)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    LH_LuaInstance *self = lh_get_self(L,lua_upvalueindex(1));

    if( self )
    {
        self->lua_pushself();
        lua_pushcclosure( L, lh_self_item_next, 1 );
        lua_newtable(L);
        lua_pushnil(L);
        Q_ASSERT( old_top+3 == lua_gettop(L) );
        return 3;
    }

    Q_ASSERT( old_top == lua_gettop(L) );
    return 0;
}

const char *LH_LuaInstance::init( lh_callback_t cb, int cb_id, const char *name, const lh_systemstate* state )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    LH_QtInstance::init(cb,cb_id,name,state);
    alc_ = LH_LuaClass::from_lh_class(cls_);
    if( alc_ == 0 ) return "can't find associated Lua class";

    // Create the 'self' table
    lua_newtable(L); // 1 self
    lua_pushliteral(L,"token"); // 2
    lua_pushlightuserdata(L,this); // 3
    lua_rawset(L,-3); // 1
    lua_pushliteral(L,"item"); // 2
    lua_newtable(L); // 3
    lua_rawset(L,-3); // 1
    lua_pushliteral(L,"module"); // 2
    alc_->lua_pushmodule(); // 3
    lua_rawset(L,-3); // 1
    lua_pushliteral(L,"callback"); // 2
    lua_pushvalue(L,-2); // 3 copy of self
    lua_pushcclosure(L,lh_lcdhost_callback,1); // 3 make sure self is available to callback
    lua_rawset(L,-3); // 1
    ref_ = luaL_ref(L,LUA_REGISTRYINDEX); // 0 pop 'self' off the stack

    Q_ASSERT( lua_gettop(L) == old_top );

    // set the self.item metatable
    lua_pushself(); // 1
    lua_pushliteral(L,"item"); // 2
    lua_rawget(L,-2); // 2 self.item
    lua_newtable(L); // 3 self.item metatable
    lua_pushliteral(L,"__index"); // 4
    lua_pushself(); // 5
    lua_pushcclosure(L,lh_self_item_index,1); // 5
    lua_rawset(L,-3); // 3
    lua_pushliteral(L,"__newindex"); // 4
    lua_pushself(); // 5
    lua_pushcclosure(L,lh_self_item_newindex,1); // 5
    lua_rawset(L,-3); // 3
    lua_pushliteral(L,"__pairs"); // 4
    lua_pushself(); // 5
    lua_pushcclosure(L,lh_self_item_pairs,1); // 5
    lua_rawset(L,-3); // 3
    lua_setmetatable(L,-2); // 2
    lua_pop(L,2); // 0 pop self & self.item

    Q_ASSERT( lua_gettop(L) == old_top );

    if( lua_pushfunction("obj_new") )
    {
        lua_pushself();
        if( lua_pcall(L,1,0,0) == 0 )
        {
            // successful call to obj_new(),
            // so enter it into lcdhost.instance
            lua_getglobal(L,"lcdhost");
            lua_pushliteral(L,"instance");
            lua_rawget(L,-2);
            lua_pushlightuserdata(L,this);
            lua_pushself();
            lua_rawset(L,-3);
            lua_pop(L,2); // lcdhost and lcdhost.instance
        }
        else
        {
            // module.obj_new caused an error
            qDebug() << "LH_Lua:" << alc_->id() << "obj_new() failed:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
    }
    else
    {
        qDebug() << "LH_Lua:" << alc_->id() << "has no obj_new()";
    }

    Q_ASSERT( lua_gettop(L) == old_top );
    return 0;
}

void LH_LuaInstance::term()
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif

    // remove from LCDHost.Instance
    lua_getglobal(L,"lcdhost");
    lua_pushliteral(L,"instance");
    lua_rawget(L,-2);
    lua_pushlightuserdata(L,this);
    lua_pushnil(L);
    lua_rawset(L,-3);
    lua_pop(L,2); // lcdhost and lcdhost.instance

    if( lua_pushfunction("obj_delete") )
    {
        lua_pushself();
        if( lua_pcall(L,1,0,0) )
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
    }

    // remove 'self' from the registry
    luaL_unref(L, LUA_REGISTRYINDEX, ref_);
    ref_ = LUA_NOREF;

    if( blob_ ) { free( blob_ ); blob_ = NULL; }
    alc_ = NULL;

    Q_ASSERT( lua_gettop(L) == old_top );
    L = NULL;
}

int LH_LuaInstance::polling()
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    int retv = 0;

    if( lua_pushfunction("obj_polling") )
    {
        push(this);
        lua_pushself();
        if( lua_pcall(L,1,1,0) )
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
        else
            retv = lua_tointeger(L,-1);
        lua_pop(L,1); // return value
        pop();
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return retv;
}

int LH_LuaInstance::notify( int code, void* )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    QByteArray codestr;
    int retv = 0;
    if( lua_pushfunction("obj_notify") )
    {
        push(this);
        lua_pushself();
        if( code & LH_NOTE_SECOND ) codestr.append('s');
        if( code & LH_NOTE_CPU ) codestr.append('c');
        if( code & LH_NOTE_MEM ) codestr.append('m');
        if( code & LH_NOTE_NET ) codestr.append('n');
        lua_pushlstring(L,codestr.constData(),codestr.size());
        if( lua_pcall(L,2,1,0) )
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
        else
        {
            const char *filter = lua_tostring(L,-1);
            while( filter && *filter )
            {
                switch( *filter )
                {
                case 's': retv |= LH_NOTE_SECOND; break;
                case 'c': retv |= LH_NOTE_CPU; break;
                case 'm': retv |= LH_NOTE_MEM; break;
                case 'n': retv |= LH_NOTE_NET; break;
                }
                ++ filter;
            }
        }
        lua_pop(L,1); // return value
        pop();
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return retv;
}

void LH_LuaInstance::prerender()
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    if( lua_pushfunction("obj_prerender") )
    {
        push(this);
        lua_pushself();
        if( lua_pcall(L,1,0,0) )
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
        pop();
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return;
}

int LH_LuaInstance::width( int forheight )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    int retv = alc_->classInfo()->width;
    if( lua_pushfunction("obj_width") )
    {
        push(this);
        lua_pushself();
        lua_pushinteger(L,forheight);
        if( lua_pcall(L,2,1,0) )
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
        else
        {
            if( lua_isnumber(L,-1) ) retv = lua_tointeger(L,-1);
        }
        pop();
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return retv;
}

int LH_LuaInstance::height( int forwidth )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    int retv = alc_->classInfo()->height;
    if( lua_pushfunction("obj_height") )
    {
        push(this);
        lua_pushself();
        lua_pushinteger(L,forwidth);
        if( !lua_pcall(L,2,1,0) )
        {
            if( lua_isnumber(L,-1) ) retv = lua_tointeger(L,-1);
        }
        else
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
        pop();
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return retv;
}

QImage *LH_LuaInstance::render_qimage( int w, int h )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    if( lua_pushfunction("obj_render") )
    {
        if( initImage(w,h) )
        {
            image_->fill( qRgba(0,0,0,0) );
            int stride = cairo_format_stride_for_width( CAIRO_FORMAT_ARGB32, w );
            Q_ASSERT( stride == image_->bytesPerLine() );
            cairo_surface_t *s = cairo_image_surface_create_for_data(
                    image_->bits(), CAIRO_FORMAT_ARGB32, w, h, stride );
            push(this);
            lua_pushself();
            lua_pushlightuserdata(L,s);
            if( !lua_pcall(L,2,0,0) )
            {
                pop();
                cairo_surface_destroy(s);
                Q_ASSERT( lua_gettop(L) == old_top );
                return image_;
            }
            cairo_surface_destroy(s);
        }
        Q_ASSERT( lua_gettop(L) == old_top );
    }

    if( lua_pushfunction("obj_render_argb32") )
    {
        push(this);
        lua_pushself();
        lua_pushinteger(L,w);
        lua_pushinteger(L,h);
        if( !lua_pcall(L,3,3,0) )
        {
            if( lua_isstring(L,-3) )
            {
                int iw = w;
                int ih = h;
                const uchar *data;
                size_t len = 0;
                if( lua_isnumber(L,-2) ) iw = lua_tointeger(L,-2);
                if( lua_isnumber(L,-1) ) ih = lua_tointeger(L,-1);
                    data = (const uchar*) lua_tolstring(L,-3,&len);
                    if( data && len > 0 && iw>=0 && ih>=0 )
                    {
                        if( initImage(iw,ih) )
                        {
                            if( image_->byteCount() == (int) len )
                            {
                                memcpy( image_->bits(), data, len );
                                lua_pop(L,3);
                                pop();
                                Q_ASSERT( lua_gettop(L) == old_top );
                                return image_;
                            }
                            else qDebug("LH_Lua: %ls: a %dx%d ARGB32 image is %d bytes, not %d",
                                        (wchar_t*) alc_->filename().utf16(), iw,ih,image_->byteCount(), (int)len);
                        }
                    }
            }
            else
            {
                if( !lua_isnil(L,-3) )
                    qDebug() << "LH_Lua:" << alc_->filename() << "obj_render_argb32() returned a" << lua_typename(L,lua_type(L,-3) ) << lua_objlen(L,-3);
                lua_pop(L,3);
                pop();
            }
        }
        else
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
            pop();
        }
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return NULL;
}

lh_blob *LH_LuaInstance::render_blob( int w, int h )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    if( lua_pushfunction("obj_render_png") )
    {
        lua_pushself();
        lua_pushinteger(L,w);
        lua_pushinteger(L,h);
        if( !lua_pcall(L,3,1,0) )
        {
            const uchar *data;
            size_t len = 0;
            data = (const uchar*) lua_tolstring(L,-1,&len);
            if( data && len > 0 )
            {
                if( blob_ ) { free( blob_ ); blob_ = NULL; }
                blob_ = (lh_blob*) malloc( sizeof(lh_blob) + len );
                if( blob_ )
                {
                    blob_->len = len;
                    blob_->sign = 0xDEADBEEF;
                    memcpy( &blob_->data, data, len );
                    Q_ASSERT( lua_gettop(L) == old_top+1 );
                    return blob_;
                }
            }
        }
        else
        {
            qDebug() << "LH_Lua:" << lua_tostring(L,-1);
            lua_pop(L,1);
        }
    }
    Q_ASSERT( lua_gettop(L) == old_top );
    return NULL;
}


static int compareSetupItems( const lua_setup_item *a, const lua_setup_item *b )
{
    return a->order() < b->order();
}

lh_setup_item **LH_LuaInstance::setup_data()
{
    QList<lua_setup_item*> list = items_;
    qStableSort( list.begin(), list.end(), compareSetupItems );

    setup_item_vector_.clear();
    foreach( lua_setup_item *si, list ) setup_item_vector_.append( si->item() );
    setup_item_vector_.append( NULL );

    return setup_item_vector_.data();
}

void LH_LuaInstance::setup_resize( lh_setup_item *item, size_t needed )
{
    foreach( lua_setup_item* si, items_ )
    {
        if( si && si->item() == item )
        {
            si->setup_resize(needed);
            return;
        }
    }
    Q_ASSERT(0);
    return;
}

void LH_LuaInstance::setup_change( lh_setup_item *item )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    foreach( lua_setup_item* si, items_ )
    {
        if( si && si->item() == item )
        {
            if( item->flags & LH_FLAG_AUTORENDER ) requestRender();
            if( lua_pushfunction("obj_setup_change") )
            {
                lua_pushself();
                lua_pushstring(L,si->key());
                if( lua_pcall(L,2,0,0) )
                {
                    qDebug() << "LH_Lua:" << lua_tostring(L,-1);
                    lua_pop(L,1);
                }
            }
            Q_ASSERT( lua_gettop(L) == old_top );
            return;
        }
    }
    Q_ASSERT(0);
    return;
}

void LH_LuaInstance::setup_input( lh_setup_item *item, int flags, int value )
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    foreach( lua_setup_item* si, items_ )
    {
        if( si && si->item() == item )
        {
            if( lua_pushfunction("obj_setup_input") )
            {
                push(this);
                lua_pushself();
                lua_pushstring(L,si->name().constData());
                lua_pushnumber(L,flags);
                lua_pushnumber(L,value);
                if( lua_pcall(L,4,0,0) )
                {
                    qDebug() << "LH_Lua:" << lua_tostring(L,-1);
                    lua_pop(L,1);
                }
                pop();
            }
            Q_ASSERT( lua_gettop(L) == old_top );
            return;
        }
    }
    Q_ASSERT(0);
    return;
}

bool LH_LuaInstance::lua_pushfunction(const char *funcname)
{
#ifndef QT_NO_DEBUG
    int old_top = lua_gettop(L);
#endif
    alc_->lua_pushmodule();
    lua_pushstring(L,funcname);
    lua_rawget(L,-2); // fetch the function
    lua_replace(L,-2);
    if( lua_isfunction(L,-1) )
    {
        Q_ASSERT( lua_gettop(L) == old_top+1 );
        return true;
    }
    lua_pop(L,1);
    Q_ASSERT( lua_gettop(L) == old_top );
    return false;
}
