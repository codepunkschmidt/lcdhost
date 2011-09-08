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

class LH_LuaVariant : public LH_QVariant
{
public:
    LH_LuaVariant() :
        LH_QVariant()
    { }

    LH_LuaVariant( const LH_LuaVariant& other ) :
        LH_QVariant(other)
    { }

    LH_LuaVariant( const LH_QVariant& val ) :
        LH_QVariant(val)
    { }

    LH_LuaVariant( lua_State *L );

    void push( lua_State *L );
};

#endif // LH_LUAVARIANT_H
