/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// XcbSurface userdata for oo usage
//---------------------------------------------------------------------------


#if CAIRO_HAS_XCB_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE


static int new_XcbSurface (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.XcbSurface

    //FIXME
    //{"create",                                   l_cairo_xcb_surface_create},
    //{"create_for_bitmap",                        l_cairo_xcb_surface_create_for_bitmap},
    //{"create_with_xrender_format",               l_cairo_xcb_surface_create_with_xrender_format},

    cairo_surface_t* cs = NULL;

    return new_Surface(L, LUACAIRO ".XcbSurface.mt", cs, CAIRO_SURFACE_TYPE_XCB, 1);
}

static const struct luaL_Reg XcbSurface_lib_f [] = {
    {"new", new_XcbSurface},
    {NULL, NULL}
};

static const struct luaL_Reg XcbSurface_lib_m [] = {
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

  //{"create",                                   l_cairo_xcb_surface_create},
  //{"create_for_bitmap",                        l_cairo_xcb_surface_create_for_bitmap},
    {"set_size",                                 l_cairo_xcb_surface_set_size},
  //{"create_with_xrender_format",               l_cairo_xcb_surface_create_with_xrender_format},

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"get_device",                               l_cairo_surface_get_device},
#endif

    {NULL, NULL}
};

static int luaopen_lXcbSurface (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".XcbSurface.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, XcbSurface_lib_m);
    luaL_register(L, LUACAIRO ".XcbSurface", XcbSurface_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_XcbSurface);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_XCB_SURFACE */



static int luaopen_lcairo_ud_xcb_surface (lua_State* L)
{
#if CAIRO_HAS_XCB_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lXcbSurface (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_XCB_SURFACE */
    return 0;
}
