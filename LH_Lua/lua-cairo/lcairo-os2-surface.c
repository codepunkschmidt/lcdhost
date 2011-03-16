/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_OS2_SURFACE
#include <cairo-os2.h>



/* The OS/2 Specific Cairo API */

// cairo_public void
// cairo_os2_init (void);
static int l_cairo_os2_init(lua_State* L)
{
    cairo_os2_init ();
    return 0;
}


// cairo_public void
// cairo_os2_fini (void);
static int l_cairo_os2_fini(lua_State* L)
{
    cairo_os2_fini ();
    return 0;
}


// cairo_public cairo_surface_t *
// cairo_os2_surface_create (HPS hps_client_window,
//                           int width,
//                           int height);
static int l_cairo_os2_surface_create(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_os2_surface_set_hwnd (cairo_surface_t *surface,
//                             HWND             hwnd_client_window);
static int l_cairo_os2_surface_set_hwnd(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public int
// cairo_os2_surface_set_size (cairo_surface_t *surface,
//                             int              new_width,
//                             int              new_height,
//                             int              timeout);
static int l_cairo_os2_surface_set_size(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    int new_width = luaL_checkinteger(L, 2);
    int new_height = luaL_checkinteger(L, 3);
    int timeout = luaL_checkinteger(L, 4);
    int v = cairo_os2_surface_set_size (surface, new_width, new_height, timeout);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void
// cairo_os2_surface_refresh_window (cairo_surface_t *surface,
//                                   HPS              hps_begin_paint,
//                                   PRECTL           prcl_begin_paint_rect);
static int l_cairo_os2_surface_refresh_window(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_surface_t *
// cairo_os2_surface_create_for_window (HWND hwnd_client_window,
//                                      int  width,
//                                      int  height);
static int l_cairo_os2_surface_create_for_window (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_os2_surface_set_manual_window_refresh (cairo_surface_t *surface,
//                                              cairo_bool_t     manual_refresh);
static int l_cairo_os2_surface_set_manual_window_refresh(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_bool_t manual_refresh = (cairo_bool_t) luaL_checkinteger(L, 2);
    cairo_os2_surface_set_manual_window_refresh (surface, manual_refresh);
    return 0;
}


// cairo_public cairo_bool_t
// cairo_os2_surface_get_manual_window_refresh (cairo_surface_t *surface);
static int l_cairo_os2_surface_get_manual_window_refresh(lua_State* L)
{
    cairo_surface_t *surface = get_cairo_surface_t (L, 1);
    cairo_bool_t v = cairo_os2_surface_get_manual_window_refresh (surface);
    lua_pushboolean(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_os2_surface_get_hps (cairo_surface_t *surface,
//                            HPS             *hps);
static int l_cairo_os2_surface_get_hps (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_status_t
// cairo_os2_surface_set_hps (cairo_surface_t *surface,
//                            HPS              hps);
static int l_cairo_os2_surface_set_hps (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg os2_f [] = {
    {"os2_init",                                 l_cairo_os2_init},
    {"os2_fini",                                 l_cairo_os2_fini},
    {"os2_surface_create",                       l_cairo_os2_surface_create},
    {"os2_surface_create_for_window",            l_cairo_os2_surface_create_for_window},
    {"os2_surface_set_hwnd",                     l_cairo_os2_surface_set_hwnd},
    {"os2_surface_set_size",                     l_cairo_os2_surface_set_size},
    {"os2_surface_refresh_window",               l_cairo_os2_surface_refresh_window},
    {"os2_surface_set_manual_window_refresh",    l_cairo_os2_surface_set_manual_window_refresh},
    {"os2_surface_get_manual_window_refresh",    l_cairo_os2_surface_get_manual_window_refresh},
    {"os2_surface_get_hps",                      l_cairo_os2_surface_get_hps},
    {"os2_surface_set_hps ",                     l_cairo_os2_surface_set_hps},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_OS2_SURFACE */


static int luaopen_lcairo_os2_surface (lua_State* L)
{
#if CAIRO_HAS_OS2_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, os2_f);

#endif /* CAIRO_HAS_OS2_SURFACE */

    return 0;
}
