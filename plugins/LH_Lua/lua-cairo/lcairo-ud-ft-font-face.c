/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// FtFontFace userdata for oo usage
// --------------------------------------------------------------------------



#if CAIRO_HAS_FT_FONT
#if LUACAIRO_HAS_OO_INTERFACE

static int new_FtFontFace (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.FtFontFace

    //FIXME
    //{"ft_font_face_create_for_pattern",          l_cairo_ft_font_face_create_for_pattern},
    //{"ft_font_face_create_for_ft_face",          l_cairo_ft_font_face_create_for_ft_face},
    cairo_font_face_t *ff = NULL;

    return new_FontFace(L, LUACAIRO ".FtFontFace.mt", ff, CAIRO_FONT_TYPE_FT, 1);
}

static const struct luaL_Reg FtFontFace_lib_f [] = {
    {"new", new_FtFontFace},
    {NULL, NULL}
};

static const struct luaL_Reg FtFontFace_lib_m [] = {
    {"__gc",                                     gc_FontFace},
    {"__tostring",                               tostring_FontFace},

    {"reference",                                l_cairo_font_face_reference},
    {"destroy",                                  gc_FontFace},
    {"get_reference_count",                      l_cairo_font_face_get_reference_count},
    {"status",                                   l_cairo_font_face_status},
    {"get_type",                                 l_cairo_font_face_get_type},
    {"get_user_data",                            l_cairo_font_face_get_user_data},
    {"set_user_data",                            l_cairo_font_face_set_user_data},

  //{"create_for_pattern",                       l_cairo_ft_font_face_create_for_pattern},
  //{"ft_font_options_substitute",               l_cairo_ft_font_options_substitute},
  //{"create_for_ft_face",                       l_cairo_ft_font_face_create_for_ft_face},

    {NULL, NULL}
};

static int luaopen_lFtFontFace (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".FtFontFace.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, FtFontFace_lib_m);
    luaL_register(L, LUACAIRO ".FtFontFace", FtFontFace_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_FtFontFace);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_FT_FONT */



static int luaopen_lcairo_ud_ft_font_face(lua_State* L)
{
#if CAIRO_HAS_FT_FONT
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lFtFontFace (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_FT_FONT */
    return 0;
}
