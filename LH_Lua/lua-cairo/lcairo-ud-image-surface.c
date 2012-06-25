/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



// --------------------------------------------------------------------------
// ImageSurface userdata for oo usage
// --------------------------------------------------------------------------



#if CAIRO_HAS_IMAGE_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE


static int new_ImageSurface (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.ImageSurface

    //FIXME
    //{"create_for_data",                          l_cairo_image_surface_create_for_data},
    //{"create_from_png",                          l_cairo_image_surface_create_from_png},
    //{"create_from_png_stream",                   l_cairo_image_surface_create_from_png_stream},

    //{"create",                                   l_cairo_image_surface_create},
    cairo_format_t format = (cairo_format_t) luaL_checkinteger(L, 1);
    int width = luaL_checkinteger(L, 2);
    int height = luaL_checkinteger(L, 3);
    cairo_surface_t *cs = cairo_image_surface_create (format, width, height);

    return new_Surface(L, LUACAIRO ".ImageSurface.mt", cs, CAIRO_SURFACE_TYPE_IMAGE, 1);
}

static const struct luaL_Reg ImageSurface_lib_f [] = {
    {"new", new_ImageSurface},
    {NULL, NULL}
};

static const struct luaL_Reg ImageSurface_lib_m [] = {
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
  //{"create",                                   l_cairo_image_surface_create},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"format_stride_for_width",                  l_cairo_format_stride_for_width},
#endif
  //{"create_for_data",                          l_cairo_image_surface_create_for_data},
    {"get_data",                                 l_cairo_image_surface_get_data},
    {"get_format",                               l_cairo_image_surface_get_format},
    {"get_width",                                l_cairo_image_surface_get_width},
    {"get_height",                               l_cairo_image_surface_get_height},
    {"get_stride",                               l_cairo_image_surface_get_stride},
#if CAIRO_HAS_PNG_FUNCTIONS
  //{"create_from_png",                          l_cairo_image_surface_create_from_png},
  //{"create_from_png_stream",                   l_cairo_image_surface_create_from_png_stream},
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"get_device",                               l_cairo_surface_get_device},
#endif

    {NULL, NULL}
};


static int luaopen_lImageSurface (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".ImageSurface.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, ImageSurface_lib_m);
    luaL_register(L, LUACAIRO ".ImageSurface", ImageSurface_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_ImageSurface);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_IMAGE_SURFACE */



static int luaopen_lcairo_ud_image_surface(lua_State* L)
{
#if CAIRO_HAS_IMAGE_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lImageSurface (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_IMAGE_SURFACE */
    return 0;
}
