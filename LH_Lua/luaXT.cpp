
#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lauxlib.h"
#ifdef __cplusplus
}
#endif

/* This simple replacement for the standard ipairs is probably
 * almost as efficient, and will work on anything which implements
 * integer keys. The prototype is ipairs(obj, [start]); if start
 * is omitted, it defaults to 1.
 *
 * Semantic differences from ipairs:
 *   1) metamethods are respected, so it will work on pseudo-arrays
 *   2) You can specify a starting point
 *   3) ipairs does not throw an error if applied to a non-table;
 *      the error will be thrown by the inext auxiliary function
 *      (if the object has no __index meta). In practice, this
 *      shouldn't make much difference except that the debug library
 *      won't figure out the name of the object.
 *   4) The auxiliary function does no explicit error checking
 *      (although it calls lua_gettable which can throw an error).
 *      If you call the auxiliary function with a non-numeric key, it
 *      will just start at 1.
 */

static int luaXT_inext (lua_State *L) {
  lua_Number n = lua_tonumber(L, 2) + 1;
  lua_pushnumber(L, n);
  lua_pushnumber(L, n);
  lua_gettable(L, 1);
  return lua_isnil(L, -1) ? 0 : 2;
}

/* Requires luaXT_inext as upvalue 1 */
static int luaXT_ipairs (lua_State *L) {
  int n = luaL_optinteger(L, 2, 1) - 1;
  luaL_checkany(L, 1);
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushvalue(L, 1);
  lua_pushinteger(L, n);
  return 3;
}

/* This could have been done with an __index metamethod for
 * strings, but that's already been used up by the string library.
 * Anyway, this is probably cleaner.
 */
static int luaXT_strnext (lua_State *L) {
  size_t len;
  const char *s = lua_tolstring(L, 1, &len);
  size_t i = (size_t) lua_tointeger(L, 2) + 1;
  if (i <= len && i > 0) {
    lua_pushinteger(L, i);
    lua_pushlstring(L, s + i - 1, 1);
    return 2;
  }
  return 0;
}

/* And finally a version of pairs that respects a __pairs metamethod.
 * It knows about two default iterators: tables and strings.
 * (This could also have been done with a __pairs metamethod for
 * strings, but there was no real point.)
 */

/* requires next and strnext as upvalues 1 and 2 */
static int luaXT_pairs (lua_State *L) {
  luaL_checkany(L, 1);
  if (luaL_getmetafield(L, 1, "__pairs")) {
    lua_insert(L, 1);
    lua_call(L, lua_gettop(L) - 1, LUA_MULTRET);
    return lua_gettop(L);
  }
  else {
    switch (lua_type(L, 1)) {
      case LUA_TTABLE: lua_pushvalue(L, lua_upvalueindex(1)); break;
      case LUA_TSTRING: lua_pushvalue(L, lua_upvalueindex(2)); break;
      default: luaL_typerror(L, 1, "iterable"); break;
    }
  }
  lua_pushvalue(L, 1);
  return 2;
}


static const luaL_reg luaXT_reg[] = {
  {"inext", luaXT_inext},
  {"strnext", luaXT_strnext},
  {NULL, NULL}
};

int luaopen_xt (lua_State *L) {
  luaL_openlib(L, "xt", luaXT_reg, 0);
  lua_getfield(L, -1, "inext");
  lua_pushcclosure(L, luaXT_ipairs, 1);
  lua_pushvalue(L, -1); lua_setglobal(L, "ipairs");
  lua_setfield(L, -2, "ipairs");
  lua_getglobal(L, "next");
  lua_getfield(L, -2, "strnext");
  lua_pushcclosure(L, luaXT_pairs, 2);
  lua_pushvalue(L, -1); lua_setglobal(L, "pairs");
  lua_setfield(L, -2, "pairs");
  return 1;
}
