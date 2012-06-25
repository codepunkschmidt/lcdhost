/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_GLITZ_SURFACE
#include <cairo-glitz.h>



// cairo_public cairo_surface_t *
// cairo_glitz_surface_create (glitz_surface_t *surface);
static int l_cairo_glitz_surface_create(lua_State* L)
{
    glitz_surface_t *surface = (glitz_surface_t *) check_lightuserdata(L, 1);
    cairo_surface_t *v = cairo_glitz_surface_create (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}



static const struct luaL_Reg glitz_f [] = {
    {"glitz_surface_create",                     l_cairo_glitz_surface_create},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_GLITZ_SURFACE */



static int luaopen_lcairo_glitz_surface (lua_State* L)
{
#if CAIRO_HAS_GLITZ_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, glitz_f);

#endif /* CAIRO_HAS_GLITZ_SURFACE */

    return 0;
}
