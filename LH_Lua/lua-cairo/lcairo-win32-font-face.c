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



// cairo_public cairo_font_face_t *
// cairo_win32_font_face_create_for_logfontw (LOGFONTW *logfont);
static int l_cairo_win32_font_face_create_for_logfontw(lua_State* L)
{
    LOGFONTW *logfont = (LOGFONTW *) check_lightuserdata(L, 1);
    cairo_font_face_t *v = cairo_win32_font_face_create_for_logfontw (logfont);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_font_face_t *
// cairo_win32_font_face_create_for_hfont (HFONT font);
static int l_cairo_win32_font_face_create_for_hfont(lua_State* L)
{
    HFONT font = (HFONT) check_lightuserdata(L, 1);
    cairo_font_face_t *v = cairo_win32_font_face_create_for_hfont (font);
    lua_pushlightuserdata(L, v);
    return 1;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public cairo_font_face_t *
// cairo_win32_font_face_create_for_logfontw_hfont (LOGFONTW *logfont, HFONT font);
static int l_cairo_win32_font_face_create_for_logfontw_hfont(lua_State* L)
{
    LOGFONTW *logfont = (LOGFONTW *) check_lightuserdata(L, 1);
    HFONT font = (HFONT) check_lightuserdata(L, 2);
    cairo_font_face_t *v = cairo_win32_font_face_create_for_logfontw_hfont (logfont, font);
    lua_pushlightuserdata(L, v);
    return 1;
}
#endif



static const struct luaL_Reg win32_font_face_f [] = {
    {"win32_font_face_create_for_logfontw",      l_cairo_win32_font_face_create_for_logfontw},
    {"win32_font_face_create_for_hfont",         l_cairo_win32_font_face_create_for_hfont},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"win32_font_face_create_for_logfontw_hfont",l_cairo_win32_font_face_create_for_logfontw_hfont},
#endif
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_WIN32_FONT */
#endif /* CAIRO_HAS_WIN32_SURFACE */



static int luaopen_lcairo_win32_font_face(lua_State* L)
{
#if CAIRO_HAS_WIN32_SURFACE
#if CAIRO_HAS_WIN32_FONT

    // register functions
    luaL_register(L, LUACAIRO, win32_font_face_f);

#endif /* CAIRO_HAS_WIN32_FONT */
#endif /* CAIRO_HAS_WIN32_SURFACE */
    return 0;
}
