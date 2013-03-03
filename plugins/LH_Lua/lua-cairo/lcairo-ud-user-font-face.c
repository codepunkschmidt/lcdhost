/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// UserFontFace userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
#if LUACAIRO_HAS_OO_INTERFACE


static int new_UserFontFace (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.UserFontFace

    cairo_font_face_t *ff = cairo_user_font_face_create ();

    return new_FontFace(L, LUACAIRO ".UserFontFace.mt", ff, CAIRO_FONT_TYPE_USER, 1);
}

static const struct luaL_Reg UserFontFace_lib_f [] = {
    {"new", new_UserFontFace},
    {NULL, NULL}
};

static const struct luaL_Reg UserFontFace_lib_m [] = {
    {"__gc",                                     gc_FontFace},
    {"__tostring",                               tostring_FontFace},

    {"reference",                                l_cairo_font_face_reference},
    {"destroy",                                  gc_FontFace},
    {"get_reference_count",                      l_cairo_font_face_get_reference_count},
    {"status",                                   l_cairo_font_face_status},
    {"get_type",                                 l_cairo_font_face_get_type},
    {"get_user_data",                            l_cairo_font_face_get_user_data},
    {"set_user_data",                            l_cairo_font_face_set_user_data},

  //{"create",                                   l_cairo_user_font_face_create},
    {"set_init_func",                            l_cairo_user_font_face_set_init_func},
    {"set_render_glyph_func",                    l_cairo_user_font_face_set_render_glyph_func},
    {"set_text_to_glyphs_func",                  l_cairo_user_font_face_set_text_to_glyphs_func},
    {"set_unicode_to_glyph_func",                l_cairo_user_font_face_set_unicode_to_glyph_func},
    {"get_init_func",                            l_cairo_user_font_face_get_init_func},
    {"get_render_glyph_func",                    l_cairo_user_font_face_get_render_glyph_func},
    {"get_text_to_glyphs_func",                  l_cairo_user_font_face_get_text_to_glyphs_func},
    {"get_unicode_to_glyph_func",                l_cairo_user_font_face_get_unicode_to_glyph_func},

    {NULL, NULL}
};

static int luaopen_lUserFontFace (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".UserFontFace.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, UserFontFace_lib_m);
    luaL_register(L, LUACAIRO ".UserFontFace", UserFontFace_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_UserFontFace);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */



static int luaopen_lcairo_ud_user_font_face(lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lUserFontFace (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */
    return 0;
}
