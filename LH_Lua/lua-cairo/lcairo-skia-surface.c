/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_SKIA_SURFACE
#include <cairo-skia.h>




// cairo_public cairo_surface_t *
// cairo_skia_surface_create (cairo_format_t format,
// 			   int width,
// 			   int height);
static int l_cairo_skia_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_skia_surface_create_for_data (unsigned char *data,
// 				    cairo_format_t format,
// 				    int width,
// 				    int height,
// 				    int stride);
static int l_cairo_skia_surface_create_for_data (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public unsigned char *
// cairo_skia_surface_get_data (cairo_surface_t *surface);
static int l_cairo_skia_surface_get_data (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_format_t
// cairo_skia_surface_get_format (cairo_surface_t *surface);
static int l_cairo_skia_surface_get_format (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_skia_surface_get_width (cairo_surface_t *surface);
static int l_cairo_skia_surface_get_width (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_skia_surface_get_height (cairo_surface_t *surface);
static int l_cairo_skia_surface_get_height (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_skia_surface_get_stride (cairo_surface_t *surface);
static int l_cairo_skia_surface_get_stride (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_skia_surface_get_image (cairo_surface_t *surface);
static int l_cairo_skia_surface_get_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg skia_f [] = {
    {"skia_surface_create",                        l_cairo_skia_surface_create},
    {"skia_surface_create_for_data",               l_cairo_skia_surface_create_for_data},
    {"skia_surface_get_data",                      l_cairo_skia_surface_get_data},
    {"skia_surface_get_format",                    l_cairo_skia_surface_get_format},
    {"skia_surface_get_width",                     l_cairo_skia_surface_get_width},
    {"skia_surface_get_height",                    l_cairo_skia_surface_get_height},
    {"skia_surface_get_stride",                    l_cairo_skia_surface_get_stride},
    {"skia_surface_get_image",                     l_cairo_skia_surface_get_image},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_SKIA_SURFACE */



static int luaopen_lcairo_skia_surface (lua_State* L)
{
#if CAIRO_HAS_SKIA_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, skia_f);

#endif /* CAIRO_HAS_SKIA_SURFACE */

    return 0;
}
