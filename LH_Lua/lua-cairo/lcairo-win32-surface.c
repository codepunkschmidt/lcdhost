/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_WIN32_SURFACE
#include <cairo-win32.h>



// cairo_public cairo_surface_t *
// cairo_win32_surface_create (HDC hdc);
static int l_cairo_win32_surface_create(lua_State* L)
{
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_surface_t *v = cairo_win32_surface_create(hdc);
    lua_pushlightuserdata(L, v);
    return 1;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public cairo_surface_t *
// cairo_win32_printing_surface_create (HDC hdc);
static int l_cairo_win32_printing_surface_create(lua_State* L)
{
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_surface_t *v = cairo_win32_printing_surface_create(hdc);
    lua_pushlightuserdata(L, v);
    return 1;
}
#endif


// cairo_public cairo_surface_t *
// cairo_win32_surface_create_with_ddb (HDC hdc,
//                                      cairo_format_t format,
//                                      int width,
//                                      int height);
static int l_cairo_win32_surface_create_with_ddb(lua_State* L)
{
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    cairo_surface_t *v = cairo_win32_surface_create_with_ddb (hdc, format, width, height);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_win32_surface_create_with_dib (cairo_format_t format,
//                                      int width,
//                                      int height);
static int l_cairo_win32_surface_create_with_dib(lua_State* L)
{
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    cairo_surface_t *v = cairo_win32_surface_create_with_dib (format, width, height);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public HDC
// cairo_win32_surface_get_dc (cairo_surface_t *surface);
static int l_cairo_win32_surface_get_dc(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    HDC v = cairo_win32_surface_get_dc (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_win32_surface_get_image (cairo_surface_t *surface);
static int l_cairo_win32_surface_get_image(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_surface_t *v = cairo_win32_surface_get_image (surface);
    lua_pushlightuserdata(L, v);
    return 1;
}




static const struct luaL_Reg win32_f [] = {
    {"win32_surface_create",                     l_cairo_win32_surface_create},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"win32_printing_surface_create",            l_cairo_win32_printing_surface_create},
#endif
    {"win32_surface_create_with_ddb",            l_cairo_win32_surface_create_with_ddb},
    {"win32_surface_create_with_dib",            l_cairo_win32_surface_create_with_dib},
    {"win32_surface_get_dc",                     l_cairo_win32_surface_get_dc},
    {"win32_surface_get_image",                  l_cairo_win32_surface_get_image},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_WIN32_SURFACE */



static int luaopen_lcairo_win32_surface (lua_State* L)
{
#if CAIRO_HAS_WIN32_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, win32_f);

#endif /* CAIRO_HAS_WIN32_SURFACE */

    return 0;
}
