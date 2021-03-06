/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



//---------------------------------------------------------------------------
// BeosSurface userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_HAS_BEOS_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE


static int new_BeosSurface (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.BeosSurface

    //FIXME
    //{"create_for_bitmap",                        l_cairo_beos_surface_create_for_bitmap},

    //{"create",                                   l_cairo_beos_surface_create},
    BView* view = (BView *) check_lightuserdata(L, 1);
    cairo_surface_t *cs = cairo_beos_surface_create (view);

    return new_Surface(L, LUACAIRO ".BeosSurface.mt", cs, CAIRO_SURFACE_TYPE_BEOS, 1);
}


static const struct luaL_Reg BeosSurface_lib_f [] = {
    {"new", new_BeosSurface},
    {NULL, NULL}
};


static const struct luaL_Reg BeosSurface_lib_m [] = {
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

  //{"create",                                   l_cairo_beos_surface_create},
  //{"create_for_bitmap",                        l_cairo_beos_surface_create_for_bitmap},

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"get_device",                               l_cairo_surface_get_device},
#endif
  
    {NULL, NULL}
};


static int luaopen_lBeosSurface (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".BeosSurface.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, BeosSurface_lib_m);
    luaL_register(L, LUACAIRO ".BeosSurface", BeosSurface_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_BeosSurface);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_BEOS_SURFACE */


static int luaopen_lcairo_ud_beos_surface(lua_State* L)
{
#if CAIRO_HAS_BEOS_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lBeosSurface (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_BEOS_SURFACE */
    return 0;
}
