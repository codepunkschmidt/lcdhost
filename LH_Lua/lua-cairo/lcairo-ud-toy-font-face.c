/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// ToyFontFace userdata for oo usage
// --------------------------------------------------------------------------



#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
#if LUACAIRO_HAS_OO_INTERFACE


static int new_ToyFontFace (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.ToyFontFace

    const char *family = luaL_checkstring(L, 1);
    cairo_font_slant_t slant = (cairo_font_slant_t) luaL_checkinteger(L, 2);
    cairo_font_weight_t weight = (cairo_font_weight_t) luaL_checkinteger(L, 3);
    cairo_font_face_t *ff = cairo_toy_font_face_create (family, slant, weight);

    return new_FontFace(L, LUACAIRO ".ToyFontFace.mt", ff, CAIRO_FONT_TYPE_TOY, 1);
}

static const struct luaL_Reg ToyFontFace_lib_f [] = {
    {"new", new_ToyFontFace},
    {NULL, NULL}
};

static const struct luaL_Reg ToyFontFace_lib_m [] = {
    {"__gc",                                     gc_FontFace},
    {"__tostring",                               tostring_FontFace},

    {"reference",                                l_cairo_font_face_reference},
    {"destroy",                                  gc_FontFace},
    {"get_reference_count",                      l_cairo_font_face_get_reference_count},
    {"status",                                   l_cairo_font_face_status},
    {"get_type",                                 l_cairo_font_face_get_type},
    {"get_user_data",                            l_cairo_font_face_get_user_data},
    {"set_user_data",                            l_cairo_font_face_set_user_data},

  //{"create",                                   l_cairo_toy_font_face_create},
    {"get_family",                               l_cairo_toy_font_face_get_family},
    {"get_slant",                                l_cairo_toy_font_face_get_slant},
    {"get_weight",                               l_cairo_toy_font_face_get_weight},

    {NULL, NULL}
};

static int luaopen_lToyFontFace (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".ToyFontFace.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, ToyFontFace_lib_m);
    luaL_register(L, LUACAIRO ".ToyFontFace", ToyFontFace_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_ToyFontFace);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */



static int luaopen_lcairo_ud_toy_font_face (lua_State *L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lToyFontFace (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */
    return 0;
}
