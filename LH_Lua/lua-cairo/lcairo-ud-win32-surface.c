/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// Win32Surface userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_HAS_WIN32_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE


static int new_Win32Surface (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Win32Surface

    //FIXME
    //{"create_with_ddb",                          l_cairo_win32_surface_create_with_ddb},
    //{"create_with_dib",                          l_cairo_win32_surface_create_with_dib},

    //{"create",                                   l_cairo_win32_surface_create},
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_surface_t *cs = cairo_win32_surface_create(hdc);

    return new_Surface(L, LUACAIRO ".Win32Surface.mt", cs, CAIRO_SURFACE_TYPE_WIN32, 1);
}

static const struct luaL_Reg Win32Surface_lib_f [] = {
    {"new", new_Win32Surface},
    {NULL, NULL}
};

static const struct luaL_Reg Win32Surface_lib_m [] = {
    {"__gc",                                     gc_Surface},
    {"__tostring",                               tostring_Surface},

    {"surface_create_similar",                   l_cairo_surface_create_similar},
    {"reference",                                l_cairo_surface_reference},
    {"finish",                                   l_cairo_surface_finish},
    {"destroy",                                  gc_Surface},
    {"get_reference_count",                      l_cairo_surface_get_reference_count},
    {"status",                                   l_cairo_surface_status},
    {"get_type",                                 l_cairo_surface_get_type},
    {"get_content",                              l_cairo_surface_get_content},
#if CAIRO_HAS_PNG_FUNCTIONS
    {"write_to_png",                             l_cairo_surface_write_to_png},
    {"write_to_png_stream",                      l_cairo_surface_write_to_png_stream},
#endif
    {"get_user_data",                            l_cairo_surface_get_user_data},
    {"set_user_data",                            l_cairo_surface_set_user_data},
    {"get_font_options",                         l_cairo_surface_get_font_options},
    {"flush",                                    l_cairo_surface_flush},
    {"mark_dirty",                               l_cairo_surface_mark_dirty},
    {"mark_dirty_rectangle",                     l_cairo_surface_mark_dirty_rectangle},
    {"set_device_offset",                        l_cairo_surface_set_device_offset},
    {"get_device_offset",                        l_cairo_surface_get_device_offset},
    {"set_fallback_resolution",                  l_cairo_surface_set_fallback_resolution},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"get_fallback_resolution",                  l_cairo_surface_get_fallback_resolution},
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"copy_page",                                l_cairo_surface_copy_page},
    {"show_page",                                l_cairo_surface_show_page},
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"has_show_text_glyphs",                     l_cairo_surface_has_show_text_glyphs},
#endif

  //{"create",                                   l_cairo_win32_surface_create},
  //{"create_with_ddb",                          l_cairo_win32_surface_create_with_ddb},
  //{"create_with_dib",                          l_cairo_win32_surface_create_with_dib},
    {"get_dc",                                   l_cairo_win32_surface_get_dc},
    {"get_image",                                l_cairo_win32_surface_get_image},

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"get_device",                               l_cairo_surface_get_device},
#endif

    {NULL, NULL}
};

static int luaopen_lWin32Surface (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Win32Surface.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, Win32Surface_lib_m);
    luaL_register(L, LUACAIRO ".Win32Surface", Win32Surface_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_Win32Surface);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}



//---------------------------------------------------------------------------
// Win32PrintingSurface userdata for oo usage
//---------------------------------------------------------------------------

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)

static int new_Win32PrintingSurface (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.Win32PrintingSurface

    //{"create",                                   l_cairo_win32_printing_surface_create},
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_surface_t *cs = cairo_win32_printing_surface_create(hdc);

    return new_Surface(L, LUACAIRO ".Win32PrintingSurface.mt", cs, CAIRO_SURFACE_TYPE_WIN32_PRINTING, 1);
}

static const struct luaL_Reg Win32PrintingSurface_lib_f [] = {
    {"new", new_Win32PrintingSurface},
    {NULL, NULL}
};

static const struct luaL_Reg Win32PrintingSurface_lib_m [] = {
    {"__gc",                                     gc_Surface},
    {"__tostring",                               tostring_Surface},

    {"surface_create_similar",                   l_cairo_surface_create_similar},
    {"reference",                                l_cairo_surface_reference},
    {"finish",                                   l_cairo_surface_finish},
    {"destroy",                                  gc_Surface},
    {"get_reference_count",                      l_cairo_surface_get_reference_count},
    {"status",                                   l_cairo_surface_status},
    {"get_type",                                 l_cairo_surface_get_type},
    {"get_content",                              l_cairo_surface_get_content},
#if CAIRO_HAS_PNG_FUNCTIONS
    {"write_to_png",                             l_cairo_surface_write_to_png},
    {"write_to_png_stream",                      l_cairo_surface_write_to_png_stream},
#endif
    {"get_user_data",                            l_cairo_surface_get_user_data},
    {"set_user_data",                            l_cairo_surface_set_user_data},
    {"get_font_options",                         l_cairo_surface_get_font_options},
    {"flush",                                    l_cairo_surface_flush},
    {"mark_dirty",                               l_cairo_surface_mark_dirty},
    {"mark_dirty_rectangle",                     l_cairo_surface_mark_dirty_rectangle},
    {"set_device_offset",                        l_cairo_surface_set_device_offset},
    {"get_device_offset",                        l_cairo_surface_get_device_offset},
    {"set_fallback_resolution",                  l_cairo_surface_set_fallback_resolution},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"get_fallback_resolution",                  l_cairo_surface_get_fallback_resolution},
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"copy_page",                                l_cairo_surface_copy_page},
    {"show_page",                                l_cairo_surface_show_page},
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"has_show_text_glyphs",                     l_cairo_surface_has_show_text_glyphs},
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
  //{"create",                                   l_cairo_win32_printing_surface_create},
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"get_device",                               l_cairo_surface_get_device},
#endif

    {NULL, NULL}
};

static int luaopen_lWin32PrintingSurface (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".Win32PrintingSurface.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, Win32PrintingSurface_lib_m);
    luaL_register(L, LUACAIRO ".Win32PrintingSurface", Win32PrintingSurface_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_Win32PrintingSurface);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}

#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0) */


#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_WIN32_SURFACE */



static int luaopen_lcairo_ud_win32_surface (lua_State* L)
{
#if CAIRO_HAS_WIN32_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lWin32Surface (L);
    
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    luaopen_lWin32PrintingSurface (L);
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0) */

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_WIN32_SURFACE */
    return 0;
}
