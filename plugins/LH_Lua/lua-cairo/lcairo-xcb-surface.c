/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_XCB_SURFACE
#include <cairo-xcb.h>



// cairo_public cairo_surface_t *
// cairo_xcb_surface_create (xcb_connection_t *c,
//               xcb_drawable_t	 drawable,
//               xcb_visualtype_t *visual,
//               int		 width,
//               int		 height);
static int l_cairo_xcb_surface_create(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_surface_t *
// cairo_xcb_surface_create_for_bitmap (xcb_connection_t *c,
//                      xcb_pixmap_t	    bitmap,
//                      xcb_screen_t	   *screen,
//                      int	    width,
//                      int	    height);
static int l_cairo_xcb_surface_create_for_bitmap(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_xcb_surface_set_size (cairo_surface_t *surface,
//                 int		     width,
//                 int		     height);
static int l_cairo_xcb_surface_set_size(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    cairo_xcb_surface_set_size (surface, width, height);
    return 0;
}


#if CAIRO_HAS_XLIB_XRENDER_SURFACE
// cairo_public cairo_surface_t *
// cairo_xcb_surface_create_with_xrender_format (xcb_connection_t	    *c,
//                           xcb_drawable_t	     drawable,
//                           xcb_screen_t		    *screen,
//                           xcb_render_pictforminfo_t *format,
//                           int		     width,
//                           int		     height);
static int l_cairo_xcb_surface_create_with_xrender_format(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


/* debug interface */

// cairo_public void
// cairo_xcb_device_debug_cap_xshm_version (cairo_device_t *device,
//                                          int major_version,
//                                          int minor_version);
static int l_cairo_xcb_device_debug_cap_xshm_version (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_xcb_device_debug_cap_xrender_version (cairo_device_t *device,
//                                             int major_version,
//                                             int minor_version);
static int l_cairo_xcb_device_debug_cap_xrender_version (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg xcb_f [] = {
    {"xcb_surface_create",                       l_cairo_xcb_surface_create},
    {"xcb_surface_create_for_bitmap",            l_cairo_xcb_surface_create_for_bitmap},
    {"xcb_surface_set_size",                     l_cairo_xcb_surface_set_size},
#if CAIRO_HAS_XLIB_XRENDER_SURFACE
    {"xcb_surface_create_with_xrender_format",   l_cairo_xcb_surface_create_with_xrender_format},
#endif
    {"xcb_device_debug_cap_xshm_version",        l_cairo_xcb_device_debug_cap_xshm_version},
    {"xcb_device_debug_cap_xrender_version",     l_cairo_xcb_device_debug_cap_xrender_version},
	{NULL, NULL} /* sentinel */
};



#endif /* CAIRO_HAS_XCB_SURFACE */



static int luaopen_lcairo_xcb_surface (lua_State* L)
{
#if CAIRO_HAS_XCB_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, xcb_f);

#endif /* CAIRO_HAS_XCB_SURFACE */

    return 0;
}
