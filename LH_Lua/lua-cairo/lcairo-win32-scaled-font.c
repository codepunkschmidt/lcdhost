/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_WIN32_SURFACE
#if CAIRO_HAS_WIN32_FONT
#include <cairo-win32.h>



// cairo_public cairo_status_t
// cairo_win32_scaled_font_select_font (cairo_scaled_font_t *scaled_font,
//                      HDC                  hdc);
static int l_cairo_win32_scaled_font_select_font(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    HDC hdc = (HDC) check_lightuserdata(L, 2);
    cairo_status_t v = cairo_win32_scaled_font_select_font (scaled_font, hdc);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_win32_scaled_font_done_font (cairo_scaled_font_t *scaled_font);
static int l_cairo_win32_scaled_font_done_font(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_win32_scaled_font_done_font (scaled_font);
    return 0;
}


// cairo_public double
// cairo_win32_scaled_font_get_metrics_factor (cairo_scaled_font_t *scaled_font);
static int l_cairo_win32_scaled_font_get_metrics_factor(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    double v = cairo_win32_scaled_font_get_metrics_factor (scaled_font);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public void
// cairo_win32_scaled_font_get_logical_to_device (cairo_scaled_font_t *scaled_font,
//                            cairo_matrix_t *logical_to_device);
static int l_cairo_win32_scaled_font_get_logical_to_device(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_matrix_t *logical_to_device = get_cairo_matrix_t (L, 2);
    cairo_win32_scaled_font_get_logical_to_device (scaled_font, logical_to_device);
    return 0;
}


// cairo_public void
// cairo_win32_scaled_font_get_device_to_logical (cairo_scaled_font_t *scaled_font,
//                            cairo_matrix_t *device_to_logical);
static int l_cairo_win32_scaled_font_get_device_to_logical(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_matrix_t *device_to_logical = get_cairo_matrix_t (L, 2);
    cairo_win32_scaled_font_get_device_to_logical (scaled_font, device_to_logical);
    return 0;
}


static const struct luaL_Reg win32_scaled_font_f [] = {
    {"win32_scaled_font_select_font",            l_cairo_win32_scaled_font_select_font},
    {"win32_scaled_font_done_font",              l_cairo_win32_scaled_font_done_font},
    {"win32_scaled_font_get_metrics_factor",     l_cairo_win32_scaled_font_get_metrics_factor},
    {"win32_scaled_font_get_logical_to_device",  l_cairo_win32_scaled_font_get_logical_to_device},
    {"win32_scaled_font_get_device_to_logical",  l_cairo_win32_scaled_font_get_device_to_logical},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_WIN32_FONT */
#endif /* CAIRO_HAS_WIN32_SURFACE */



static int luaopen_lcairo_win32_scaled_font(lua_State* L)
{
#if CAIRO_HAS_WIN32_SURFACE
#if CAIRO_HAS_WIN32_FONT

    // register functions
    luaL_register(L, LUACAIRO, win32_scaled_font_f);

#endif /* CAIRO_HAS_WIN32_FONT */
#endif /* CAIRO_HAS_WIN32_SURFACE */

    return 0;
}
