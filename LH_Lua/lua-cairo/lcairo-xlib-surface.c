/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_XLIB_SURFACE
#include <cairo-xlib.h>



// cairo_public cairo_surface_t *
// cairo_xlib_surface_create (Display     *dpy,
//                Drawable	drawable,
//                Visual      *visual,
//                int		width,
//                int		height);
static int l_cairo_xlib_surface_create(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_surface_t *
// cairo_xlib_surface_create_for_bitmap (Display  *dpy,
//                       Pixmap	bitmap,
//                       Screen	*screen,
//                       int	width,
//                       int	height);
static int l_cairo_xlib_surface_create_for_bitmap(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_xlib_surface_set_size (cairo_surface_t *surface,
//                  int              width,
//                  int              height);
static int l_cairo_xlib_surface_set_size(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    cairo_xlib_surface_set_size (surface, width, height);
    return 0;
}


// cairo_public void
// cairo_xlib_surface_set_drawable (cairo_surface_t *surface,
//                  Drawable	  drawable,
//                  int              width,
//                  int              height);
static int l_cairo_xlib_surface_set_drawable(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public Display *
// cairo_xlib_surface_get_display (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_display(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    Display *v = cairo_xlib_surface_get_display (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public Drawable
// cairo_xlib_surface_get_drawable (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_drawable(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public Screen *
// cairo_xlib_surface_get_screen (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_screen(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    Screen *v = cairo_xlib_surface_get_screen (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public Visual *
// cairo_xlib_surface_get_visual (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_visual(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    Visual *v = cairo_xlib_surface_get_visual (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public int
// cairo_xlib_surface_get_depth (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_depth(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int v = cairo_xlib_surface_get_depth (surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public int
// cairo_xlib_surface_get_width (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_width(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int v = cairo_xlib_surface_get_width (surface);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public int
// cairo_xlib_surface_get_height (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_height(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int v = cairo_xlib_surface_get_height (surface);
    lua_pushinteger(L, v);
    return 1;
}


#if CAIRO_HAS_XLIB_XRENDER_SURFACE
// cairo_public cairo_surface_t *
// cairo_xlib_surface_create_with_xrender_format (Display		 *dpy,
//                                                Drawable		  drawable,
//                            Screen		 *screen,
//                                                XRenderPictFormat *format,
//                                                int		  width,
//                                                int		  height);
static int l_cairo_xlib_surface_create_with_xrender_format(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public XRenderPictFormat *
// cairo_xlib_surface_get_xrender_format (cairo_surface_t *surface);
static int l_cairo_xlib_surface_get_xrender_format (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


static const struct luaL_Reg xlib_f [] = {
    {"xlib_surface_create",                      l_cairo_xlib_surface_create},
    {"xlib_surface_create_for_bitmap",           l_cairo_xlib_surface_create_for_bitmap},
    {"xlib_surface_set_size",                    l_cairo_xlib_surface_set_size},
    {"xlib_surface_set_drawable",                l_cairo_xlib_surface_set_drawable},
    {"xlib_surface_get_display",                 l_cairo_xlib_surface_get_display},
    {"xlib_surface_get_drawable",                l_cairo_xlib_surface_get_drawable},
    {"xlib_surface_get_screen",                  l_cairo_xlib_surface_get_screen},
    {"xlib_surface_get_visual",                  l_cairo_xlib_surface_get_visual},
    {"xlib_surface_get_depth",                   l_cairo_xlib_surface_get_depth},
    {"xlib_surface_get_width",                   l_cairo_xlib_surface_get_width},
    {"xlib_surface_get_height",                  l_cairo_xlib_surface_get_height},
#if CAIRO_HAS_XLIB_XRENDER_SURFACE
    {"xlib_surface_create_with_xrender_format",  l_cairo_xlib_surface_create_with_xrender_format},
    {"xlib_surface_get_xrender_format",          l_cairo_xlib_surface_get_xrender_format},
#endif
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_XLIB_SURFACE */



static int luaopen_lcairo_xlib_surface (lua_State* L)
{
#if CAIRO_HAS_XLIB_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, xlib_f);

#endif /* CAIRO_HAS_XLIB_SURFACE */

    return 0;
}
