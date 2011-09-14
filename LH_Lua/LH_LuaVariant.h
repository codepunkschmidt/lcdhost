#ifndef LH_LUAVARIANT_H
#define LH_LUAVARIANT_H

extern "C" {
#ifdef LUA_STATIC
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
#else
# include "lua_dyn.h"
# define LUA_PREFIX LuaFunctions.
#endif
}

#include "LH_QVariant.h"

class LH_LuaVariant : public QVariant
{
public:
    LH_LuaVariant() :
        QVariant()
    { }

    LH_LuaVariant( const LH_LuaVariant& other ) :
        QVariant(other)
    { }

    LH_LuaVariant( const QVariant& val ) :
        QVariant(val)
    { }

    LH_LuaVariant( lua_State *L );

    void push( lua_State *L );
};

#endif // LH_LUAVARIANT_H
