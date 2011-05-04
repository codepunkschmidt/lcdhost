/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_QUARTZ_IMAGE_SURFACE
#include <cairo-quartz-image.h>



// cairo_public cairo_surface_t *
// cairo_quartz_image_surface_create (cairo_surface_t *image_surface);
static int l_cairo_quartz_image_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_quartz_image_surface_get_image (cairo_surface_t *surface);
static int l_cairo_quartz_image_surface_get_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg quartz_image_f [] = {
    {"quartz_image_surface_create",                  l_cairo_quartz_image_surface_create},
    {"quartz_image_surface_get_image",               l_cairo_quartz_image_surface_get_image},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_QUARTZ_IMAGE_SURFACE */



static int luaopen_lcairo_quartz_image_surface (lua_State* L)
{
#if CAIRO_HAS_QUARTZ_IMAGE_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, quartz_image_f);

#endif /* CAIRO_HAS_QUARTZ_IMAGE_SURFACE */

    return 0;
}
