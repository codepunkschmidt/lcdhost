/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_QUARTZ_SURFACE
#include <cairo-quartz.h>



// cairo_public cairo_surface_t *
// cairo_quartz_surface_create (cairo_format_t format,
//                              unsigned int width,
//                              unsigned int height);
static int l_cairo_quartz_surface_create(lua_State* L)
{
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 1);
    unsigned int width = luaL_checknumber(L, 2);
    unsigned int height = luaL_checknumber(L, 3);
    cairo_surface_t *v = cairo_quartz_surface_create (format, width, height);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_quartz_surface_create_for_cg_context (CGContextRef cgContext,
//                                             unsigned int width,
//                                             unsigned int height);
static int l_cairo_quartz_surface_create_for_cg_context(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public CGContextRef
// cairo_quartz_surface_get_cg_context (cairo_surface_t *surface);
static int l_cairo_quartz_surface_get_cg_context(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg quartz_f [] = {
    {"quartz_surface_create",                        l_cairo_quartz_surface_create},
    {"quartz_surface_create_for_cg_context",         l_cairo_quartz_surface_create_for_cg_context},
    {"quartz_surface_get_cg_context",                l_cairo_quartz_surface_get_cg_context},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_QUARTZ_SURFACE */



static int luaopen_lcairo_quartz_surface (lua_State* L)
{
#if CAIRO_HAS_QUARTZ_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, quartz_f);

#endif /* CAIRO_HAS_QUARTZ_SURFACE */

    return 0;
}
