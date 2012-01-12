/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



// --------------------------------------------------------------------------
// FtScaledFont userdata for oo usage
// --------------------------------------------------------------------------



#if CAIRO_HAS_FT_FONT
#if LUACAIRO_HAS_OO_INTERFACE


static int new_FtScaledFont (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.FtScaledFont

    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    const cairo_matrix_t *font_matrix = get_cairo_matrix_t (L, 2);
    const cairo_matrix_t *ctm = get_cairo_matrix_t (L, 3);
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 4);
    cairo_scaled_font_t *sf = cairo_scaled_font_create (font_face, font_matrix, ctm, options);

    return new_ScaledFont(L, LUACAIRO ".FtScaledFont.mt", sf, 2, 1);
}

static const struct luaL_Reg FtScaledFont_lib_f [] = {
    {"new", new_FtScaledFont},
    {NULL, NULL}
};

static const struct luaL_Reg FtScaledFont_lib_m [] = {
    {"__gc",                                     gc_ScaledFont},
    {"__tostring",                               tostring_ScaledFont},

  //{"create",                                   l_cairo_scaled_font_create},
    {"reference",                                l_cairo_scaled_font_reference},
    {"destroy",                                  gc_ScaledFont},
    {"get_reference_count",                      l_cairo_scaled_font_get_reference_count},
    {"status",                                   l_cairo_scaled_font_status},
    {"get_type",                                 l_cairo_scaled_font_get_type},
    {"get_user_data",                            l_cairo_scaled_font_get_user_data},
    {"set_user_data",                            l_cairo_scaled_font_set_user_data},
    {"extents",                                  l_cairo_scaled_font_extents},
    {"text_extents",                             l_cairo_scaled_font_text_extents},
    {"glyph_extents",                            l_cairo_scaled_font_glyph_extents},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"text_to_glyphs",                           l_cairo_scaled_font_text_to_glyphs},
#endif
    {"get_font_face",                            l_cairo_scaled_font_get_font_face},
    {"get_font_matrix",                          l_cairo_scaled_font_get_font_matrix},
    {"get_ctm",                                  l_cairo_scaled_font_get_ctm},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"get_scale_matrix",                         l_cairo_scaled_font_get_scale_matrix},
#endif
    {"get_font_options",                         l_cairo_scaled_font_get_font_options},

#if CAIRO_HAS_FT_FONT
    {"lock_face",                                l_cairo_ft_scaled_font_lock_face},
    {"unlock_face",                              l_cairo_ft_scaled_font_unlock_face},
#endif

    {NULL, NULL}
};

static int luaopen_lFtScaledFont (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".FtScaledFont.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, FtScaledFont_lib_m);
    luaL_register(L, LUACAIRO ".FtScaledFont", FtScaledFont_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_FtScaledFont);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_FT_FONT */



static int luaopen_lcairo_ud_ft_scaled_font(lua_State* L)
{
#if CAIRO_HAS_FT_FONT
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lFtScaledFont (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_FT_FONT */
    return 0;
}
