/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// QuartzFontFace userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_HAS_QUARTZ_FONT
#if LUACAIRO_HAS_OO_INTERFACE


static int new_QuartzFontFace (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.QuartzFontFace

    //FIXME
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    //{"quartz_font_face_create_for_cgfont",       l_cairo_quartz_font_face_create_for_cgfont},
    //{"quartz_font_face_create_for_atsu_font_id", l_cairo_quartz_font_face_create_for_atsu_font_id},
#endif

    cairo_font_face_t *ff = NULL;

    return new_FontFace(L, LUACAIRO ".QuartzFontFace.mt", ff, CAIRO_FONT_TYPE_QUARTZ, 1);
}

static const struct luaL_Reg QuartzFontFace_lib_f [] = {
    {"new", new_QuartzFontFace},
    {NULL, NULL}
};

static const struct luaL_Reg QuartzFontFace_lib_m [] = {
    {"__gc",                                     gc_FontFace},
    {"__tostring",                               tostring_FontFace},

    {"reference",                                l_cairo_font_face_reference},
    {"destroy",                                  gc_FontFace},
    {"get_reference_count",                      l_cairo_font_face_get_reference_count},
    {"status",                                   l_cairo_font_face_status},
    {"get_type",                                 l_cairo_font_face_get_type},
    {"get_user_data",                            l_cairo_font_face_get_user_data},
    {"set_user_data",                            l_cairo_font_face_set_user_data},

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
  //{"create_for_cgfont",                        l_cairo_quartz_font_face_create_for_cgfont},
  //{"create_for_atsu_font_id",                  l_cairo_quartz_font_face_create_for_atsu_font_id},
#endif

    {NULL, NULL}
};

static int luaopen_lQuartzFontFace (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".QuartzFontFace.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, QuartzFontFace_lib_m);
    luaL_register(L, LUACAIRO ".QuartzFontFace", QuartzFontFace_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_QuartzFontFace);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_QUARTZ_FONT */



static int luaopen_lcairo_ud_quartz_font_face(lua_State* L)
{
#if CAIRO_HAS_QUARTZ_FONT
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lQuartzFontFace (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
#endif  /* CAIRO_HAS_QUARTZ_FONT */
    return 0;
}
