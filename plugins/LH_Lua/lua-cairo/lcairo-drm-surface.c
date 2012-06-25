/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_DRM_SURFACE
#include <cairo-drm.h>



// cairo_public cairo_device_t *
// cairo_drm_device_get (struct udev_device *device);
static int l_cairo_drm_device_get (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_device_t *
// cairo_drm_device_get_for_fd (int fd);
static int l_cairo_drm_device_get_for_fd (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_device_t *
// cairo_drm_device_default (void);
static int l_cairo_drm_device_default (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_drm_device_get_fd (cairo_device_t *device);
static int l_cairo_drm_device_get_fd (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_drm_device_throttle (cairo_device_t *device);
static int l_cairo_drm_device_throttle (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_drm_surface_create (cairo_device_t *device,
// 			  cairo_format_t format,
// 			  int width, int height);
static int l_cairo_drm_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_drm_surface_create_for_name (cairo_device_t *device,
// 				   unsigned int name,
// 	                           cairo_format_t format,
// 				   int width, int height, int stride);
static int l_cairo_drm_surface_create_for_name (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_drm_surface_create_from_cacheable_image (cairo_device_t *device,
// 	                                       cairo_surface_t *surface);
static int l_cairo_drm_surface_create_from_cacheable_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_status_t
// cairo_drm_surface_enable_scan_out (cairo_surface_t *surface);
static int l_cairo_drm_surface_enable_scan_out (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public unsigned int
// cairo_drm_surface_get_handle (cairo_surface_t *surface);
static int l_cairo_drm_surface_get_handle (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public unsigned int
// cairo_drm_surface_get_name (cairo_surface_t *surface);
static int l_cairo_drm_surface_get_name (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_format_t
// cairo_drm_surface_get_format (cairo_surface_t *surface);
static int l_cairo_drm_surface_get_format (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_drm_surface_get_width (cairo_surface_t *surface);
static int l_cairo_drm_surface_get_width (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_drm_surface_get_height (cairo_surface_t *surface);
static int l_cairo_drm_surface_get_height (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_drm_surface_get_stride (cairo_surface_t *surface);
static int l_cairo_drm_surface_get_stride (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_drm_surface_map_to_image (cairo_surface_t *surface);
static int l_cairo_drm_surface_map_to_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_drm_surface_unmap (cairo_surface_t *drm_surface,
// 	                 cairo_surface_t *image_surface);
static int l_cairo_drm_surface_unmap (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg drm_surface_f [] = {
    {"drm_device_get",                          l_cairo_drm_device_get},
    {"drm_device_get_for_fd",                   l_cairo_drm_device_get_for_fd},
    {"drm_device_default",                      l_cairo_drm_device_default},
    {"drm_device_get_fd",                       l_cairo_drm_device_get_fd},
    {"drm_device_throttle",                     l_cairo_drm_device_throttle},
    {"drm_surface_create",                      l_cairo_drm_surface_create},
    {"drm_surface_create_for_name",             l_cairo_drm_surface_create_for_name},
    {"drm_surface_create_from_cacheable_image", l_cairo_drm_surface_create_from_cacheable_image},
    {"drm_surface_enable_scan_out",             l_cairo_drm_surface_enable_scan_out},
    {"drm_surface_get_handle",                  l_cairo_drm_surface_get_handle},
    {"drm_surface_get_name",                    l_cairo_drm_surface_get_name},
    {"drm_surface_get_format",                  l_cairo_drm_surface_get_format},
    {"drm_surface_get_width",                   l_cairo_drm_surface_get_width},
    {"drm_surface_get_height",                  l_cairo_drm_surface_get_height},
    {"drm_surface_get_stride",                  l_cairo_drm_surface_get_stride},
    {"drm_surface_map_to_image",                l_cairo_drm_surface_map_to_image},
    {"drm_surface_unmap",                       l_cairo_drm_surface_unmap},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_DRM_SURFACE */



static int luaopen_lcairo_drm_surface (lua_State* L)
{
#if CAIRO_HAS_DRM_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, drm_surface_f);

#endif /* CAIRO_HAS_DRM_SURFACE */

    return 0;
}
