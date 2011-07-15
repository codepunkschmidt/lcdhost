/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_QT_SURFACE
#include <cairo-qt.h>



// cairo_public cairo_surface_t *
// cairo_qt_surface_create (QPainter *painter);
static int l_cairo_qt_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_qt_surface_create_with_qimage (cairo_format_t format,
// 				     int width,
// 				     int height);
static int l_cairo_qt_surface_create_with_qimage (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_qt_surface_create_with_qpixmap (cairo_content_t content,
// 				      int width,
// 				      int height);
static int l_cairo_qt_surface_create_with_qpixmap (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public QPainter *
// cairo_qt_surface_get_qpainter (cairo_surface_t *surface);
static int l_cairo_qt_surface_get_qpainter (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_qt_surface_get_image (cairo_surface_t *surface);
static int l_cairo_qt_surface_get_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public QImage *
// cairo_qt_surface_get_qimage (cairo_surface_t *surface);
static int l_cairo_qt_surface_get_qimage (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg qt_f [] = {
    {"cairo_qt_surface_create",                        l_cairo_qt_surface_create},
    {"cairo_qt_surface_create_with_qimage",            l_cairo_qt_surface_create_with_qimage},
    {"cairo_qt_surface_create_with_qpixmap",           l_cairo_qt_surface_create_with_qpixmap},
    {"cairo_qt_surface_get_qpainter",                  l_cairo_qt_surface_get_qpainter},
    {"cairo_qt_surface_get_image",                     l_cairo_qt_surface_get_image},
    {"cairo_qt_surface_get_qimage",                    l_cairo_qt_surface_get_qimage},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_QT_SURFACE */



static int luaopen_lcairo_qt_surface (lua_State* L)
{
#if CAIRO_HAS_QT_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, qt_f);

#endif /* CAIRO_HAS_QT_SURFACE */

    return 0;
}
