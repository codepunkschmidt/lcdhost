/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// ScaledFont userdata for cairo_scaled_font_t
// --------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _ScaledFont
{
    cairo_scaled_font_t* sf_;
    int                  havesf_;
    int                  typesf_; //0:ScaledFont, 1:Win32ScaledFont, 2:FtScaledFont
} ScaledFont;


static ScaledFont* check_ScaledFont_ud(lua_State *L, int idx)
{
    ScaledFont* sf = NULL;

    if (!sf) sf = (ScaledFont*) get_userdata(L, idx, LUACAIRO ".ScaledFont.mt");
#if CAIRO_HAS_WIN32_FONT
    if (!sf) sf = (ScaledFont*) get_userdata(L, idx, LUACAIRO ".Win32ScaledFont.mt");
#endif
#if CAIRO_HAS_FT_FONT
    if (!sf) sf = (ScaledFont*) get_userdata(L, idx, LUACAIRO ".FtScaledFont.mt");
#endif
    if (!sf) luaL_typerror(L, idx, LUACAIRO ".ScaledFont");

    return sf;
}

static int new_ScaledFont (lua_State *L, const char *tname, cairo_scaled_font_t* sf, int typesf, int havesf)
{
    ScaledFont *o = (ScaledFont *) lua_newuserdata(L, sizeof(ScaledFont));
    o->sf_     = sf;
    o->havesf_ = havesf;
    o->typesf_ = typesf;

    luaL_getmetatable(L, tname);
    lua_setmetatable(L, -2);

    return 1;
}

static int new_ScaledFontFromLUD (lua_State *L)
{
    cairo_scaled_font_t *sf = (cairo_scaled_font_t *) check_lightuserdata(L, 1);
    int havesf              = lua_toboolean(L, 2);
    int typesf              = 0; //FIXME assume default ScaledFont

    switch (typesf) {
    case 0: return new_ScaledFont(L, LUACAIRO ".ScaledFont.mt", sf, typesf, havesf);
#if CAIRO_HAS_WIN32_FONT
    case 1: return new_ScaledFont(L, LUACAIRO ".Win32ScaledFont.mt", sf, typesf, havesf);
#endif
#if CAIRO_HAS_FT_FONT
    case 2: return new_ScaledFont(L, LUACAIRO ".FtScaledFont.mt", sf, typesf, havesf);
#endif
    }

    return 0;
}

static int gc_ScaledFont (lua_State *L)
{
    ScaledFont *o = check_ScaledFont_ud(L, 1);

    if (o->sf_ && o->havesf_)
    {
        cairo_scaled_font_destroy(o->sf_);
        o->sf_     = NULL;
        o->havesf_ = 0;
        o->typesf_ = 0;
    }

    return 0;
}

static int tostring_ScaledFont (lua_State *L)
{
    ScaledFont *o = check_ScaledFont_ud(L, 1);
    lua_pushfstring(L, "ScaledFont (%p), cairo_scaled_font_t (%p), Type:%d", (void*)o, (void*)o->sf_, o->typesf_);
    return 1;
}



static int new_ScaledFont_default (lua_State *L)
{
    lua_remove(L, 1); // remove cairo.ScaledFont

    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    const cairo_matrix_t *font_matrix = get_cairo_matrix_t (L, 2);
    const cairo_matrix_t *ctm = get_cairo_matrix_t (L, 3);
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 4);
    cairo_scaled_font_t *sf = cairo_scaled_font_create (font_face, font_matrix, ctm, options);

    return new_ScaledFont(L, LUACAIRO ".ScaledFont.mt", sf, 0, 1);
}

static const struct luaL_Reg ScaledFont_default_lib_f [] = {
    {"new", new_ScaledFont_default},
    {NULL, NULL}
};

static const struct luaL_Reg ScaledFont_default_lib_m [] = {
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

    {NULL, NULL}
};

static int luaopen_lScaledFont_default (lua_State *L)
{
    luaL_newmetatable(L, LUACAIRO ".ScaledFont.mt");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, ScaledFont_default_lib_m);
    luaL_register(L, LUACAIRO ".ScaledFont", ScaledFont_default_lib_f);
    lua_newtable(L);
    lua_pushstring(L, "__call");
    lua_pushcfunction(L, new_ScaledFont_default);
    lua_settable(L, -3);
    lua_setmetatable(L, -2);
    lua_pop(L, 1);
    lua_pop(L, 1);
    return 0;
}


static int luaopen_lscaledfont (lua_State *L)
{
    luaopen_lScaledFont_default (L);
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */




static int luaopen_lcairo_ud_scaled_font(lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lscaledfont (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
