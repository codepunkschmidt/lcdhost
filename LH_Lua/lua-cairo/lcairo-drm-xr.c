/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_DRM_XR_FUNCTIONS
#include <cairo-drm-xr.h>



// cairo_public xr_screen_t *
// cairo_drm_xr_enable (ScreenPtr screen, int fd);
static int l_cairo_drm_xr_enable (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_drm_xr_pixmap_link_bo (xr_screen_t *xr,
// 			     PixmapPtr pixmap,
// 			     uint32_t name,
// 			     cairo_format_t format,
// 			     int width,
// 			     int height,
// 			     int stride);
static int l_cairo_drm_xr_pixmap_link_bo (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_drm_xr_pixmap_unlink_bo (xr_screen_t *xr,
// 			       PixmapPtr pixmap);
static int l_cairo_drm_xr_pixmap_unlink_bo (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg drm_xr_f [] = {
    {"drm_xr_enable",                          l_cairo_drm_xr_enable},
    {"drm_xr_pixmap_link_bo",                  l_cairo_drm_xr_pixmap_link_bo},
    {"drm_xr_pixmap_unlink_bo",                l_cairo_drm_xr_pixmap_unlink_bo},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_DRM_XR_FUNCTIONS */



static int luaopen_lcairo_drm_xr(lua_State* L)
{
#if CAIRO_HAS_DRM_XR_FUNCTIONS

    // register functions
    luaL_register(L, LUACAIRO, drm_xr_f);

#endif /* CAIRO_HAS_DRM_XR_FUNCTIONS */

    return 0;
}
