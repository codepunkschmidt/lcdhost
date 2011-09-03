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

#include "../LH_Variant.h"

class LH_LuaVariant : public LH_Variant
{
public:
    LH_LuaVariant() :
        LH_Variant()
    { }

    LH_LuaVariant( const LH_LuaVariant& other ) :
        LH_Variant(other)
    { }

    LH_LuaVariant( const LH_Variant& val ) :
        LH_Variant(val)
    { }

    LH_LuaVariant( lua_State *L );

    void push( lua_State *L );
};

#endif // LH_LUAVARIANT_H
