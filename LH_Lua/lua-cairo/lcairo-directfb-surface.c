/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_DIRECTFB_SURFACE
#include <cairo-directfb.h>



// cairo_public cairo_surface_t *
// cairo_directfb_surface_create (IDirectFB *dfb, IDirectFBSurface *surface);
static int l_cairo_directfb_surface_create(lua_State* L)
{
    IDirectFB *dfb = (IDirectFB *) check_lightuserdata(L, 1);
    IDirectFBSurface *surface = (IDirectFBSurface *) check_lightuserdata(L, 2);
    cairo_surface_t *v = cairo_directfb_surface_create (dfb, surface);
    lua_pushlightuserdata(L, v);
    return 1;
}



static const struct luaL_Reg directfb_surface_f [] = {
    {"directfb_surface_create",                  l_cairo_directfb_surface_create},
	{NULL, NULL} /* sentinel */
};


#endif /*CAIRO_HAS_DIRECTFB_SURFACE*/



static int luaopen_lcairo_directfb_surface(lua_State* L)
{
#if CAIRO_HAS_DIRECTFB_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, directfb_surface_f);

#endif /*CAIRO_HAS_DIRECTFB_SURFACE*/

    return 0;
}
