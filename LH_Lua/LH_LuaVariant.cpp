#include "LH_LuaVariant.h"

LH_LuaVariant::LH_LuaVariant( lua_State *L )
    : LH_QVariant()
{
    switch( lua_type(L,-1) )
    {
    case LUA_TNIL:
        setValue( QVariant() );
        break;
    case LUA_TBOOLEAN:
        setValue( (bool) lua_toboolean(L,-1) );
        break;
    case LUA_TNUMBER:
        setValue( (double) lua_tonumber(L,-1) );
        break;
    case LUA_TSTRING:
        {
            size_t len = 0;
            const char *str = lua_tolstring( L, -1, &len );
            LH_QVariant::fromString( QString::fromUtf8(str, len) );
        }
        break;
    case LUA_TTABLE:
    {
        QVariantList vl;
        lua_pushnil(L);
        while( lua_next(L, -2) )
        {
            vl.append( LH_LuaVariant(L) );
            lua_pop(L, 1);
        }
        setValue( vl );
        break;
    }
    case LUA_TLIGHTUSERDATA:
    case LUA_TFUNCTION:
    case LUA_TUSERDATA:
    case LUA_TTHREAD:
    default:
        lua_pushstring(L,"unhandled Lua data type");
        lua_error(L);
        break;
    }
}

void LH_LuaVariant::push( lua_State *L )
{
    if( isNull() || !isValid() )
    {
        lua_pushnil(L);
        return;
    }
    if( type() == QVariant::LongLong || type() == QVariant::Int )
    {
        lua_pushinteger(L,LH_QVariant::toLongLong());
        return;
    }
    if( type() == QVariant::Double )
    {
        lua_pushnumber(L,LH_QVariant::toDouble());
        return;
    }
    if( type() == QVariant::List || type() == QVariant::StringList )
    {
        Q_ASSERT(0);
        return;
    }
    const QByteArray ba( LH_QVariant::toString().toUtf8() );
    lua_pushlstring(L,ba.constData(),ba.size());
    return;
}
