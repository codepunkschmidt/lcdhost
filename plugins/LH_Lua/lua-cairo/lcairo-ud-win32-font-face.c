/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// Win32FontFace userdata for oo usage
// --------------------------------------------------------------------------



#if CAIRO_HAS_WIN32_FONT
#if LUACAIRO_HAS_OO_INTERFACE


static int new_Win32FontFace (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Win32FontFace

    //FIXME
    //{"win32_font_face_create_for_logfontw",      l_cairo_win32_font_face_create_for_logfontw},
    //{"win32_font_face_create_for_hfont",         l_cairo_win32_font_face_create_for_hfont},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    //{"win32_font_face_create_for_logfontw_hfont",l_cairo_win32_font_face_create_for_logfontw_hfont},
#endif

    LOGFONTW *logfont = (LOGFONTW *) check_lightuserdata(L, 1);
    cairo_font_face_t *ff = cairo_win32_font_face_create_for_logfontw (logfont);

    return new_FontFace(L, LUACAIRO ".Win32FontFace.mt", ff, CAIRO_FONT_TYPE_WIN32, 1);
}

static const struct luaL_Reg Win32FontFace_lib_f [] = {
    {"new", new_Win32FontFace},
    {NULL, NULL}
};

static const struct luaL_Reg Win32FontFace_lib_m [] = {
    {"__gc",                                     gc_FontFace},
    {"__tostring",                               tostring_FontFace},

    {"reference",                                l_cairo_font_face_reference},
    {"destroy",                                  gc_FontFace},
    {"get_reference_count",                      l_cairo_font_face_get_reference_count},
    {"status",                                   l_cairo_font_face_status},
    {"get_type",                                 l_cairo_font_face_get_type},
    {"get_user_data",                            l_cairo_font_face_get_user_data},
    {"set_user_data",                            l_cairo_font_face_set_user_data},

  //{"create_for_logfontw",                      l_cairo_win32_font_face_create_for_logfontw},
  //{"create_for_hfont",                         l_cairo_win32_font_face_create_for_hfont},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
  //{"create_for_logfontw_hfont",                l_cairo_win32_font_face_create_for_logfontw_hfont},
#endif

    {NULL, NULL}
};

static int luaopen_lWin32FontFace (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Win32FontFace.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, Win32FontFace_lib_m);
    luaL_register(L, LUACAIRO ".Win32FontFace", Win32FontFace_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_Win32FontFace);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_WIN32_FONT */



static int luaopen_lcairo_ud_win32_font_face(lua_State* L)
{
#if CAIRO_HAS_WIN32_FONT
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lWin32FontFace (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_WIN32_FONT */
    return 0;
}
