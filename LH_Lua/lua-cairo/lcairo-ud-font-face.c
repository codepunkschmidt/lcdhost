/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




// --------------------------------------------------------------------------
// FontFace userdata for cairo_font_face_t
// --------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _FontFace
{
    cairo_font_face_t* ff_;
    int                haveff_;
    int                typeff_;
} FontFace;

static FontFace* check_FontFace_ud(lua_State *L, int idx)
{
    FontFace* ff = NULL;

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    if (!ff) ff = (FontFace*) get_userdata(L, idx, LUACAIRO ".ToyFontFace.mt");
#endif
#if CAIRO_HAS_FT_FONT
    if (!ff) ff = (FontFace*) get_userdata(L, idx, LUACAIRO ".FtFontFace.mt");
#endif
#if CAIRO_HAS_WIN32_FONT
    if (!ff) ff = (FontFace*) get_userdata(L, idx, LUACAIRO ".Win32FontFace.mt");
#endif
#if CAIRO_HAS_QUARTZ_FONT
    if (!ff) ff = (FontFace*) get_userdata(L, idx, LUACAIRO ".QuartzFontFace.mt");
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    if (!ff) ff = (FontFace*) get_userdata(L, idx, LUACAIRO ".UserFontFace.mt");
#endif
    if (!ff) luaL_typerror(L, idx, LUACAIRO ".FontFace");

    return ff;
}

static int new_FontFace (lua_State *L, const char *tname, cairo_font_face_t* ff, int typeff, int haveff)
{
    FontFace *o = (FontFace *) lua_newuserdata(L, sizeof(FontFace));
    o->ff_     = ff;
    o->haveff_ = haveff;
    o->typeff_ = typeff;

    luaL_getmetatable(L, tname);
    lua_setmetatable(L, -2);

    return 1;
}

static int new_FontFaceFromLUD (lua_State *L)
{
    cairo_font_face_t *ff = (cairo_font_face_t *) check_lightuserdata(L, 1);
    int haveff            = lua_toboolean(L, 2);
    int typeff            = cairo_font_face_get_type (ff);

    switch (typeff) {
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    case CAIRO_FONT_TYPE_TOY:    return new_FontFace(L, LUACAIRO ".ToyFontFace.mt", ff, typeff, haveff);
#endif
#if CAIRO_HAS_FT_FONT
    case CAIRO_FONT_TYPE_FT:     return new_FontFace(L, LUACAIRO ".FtFontFace.mt", ff, typeff, haveff);
#endif
#if CAIRO_HAS_WIN32_FONT
    case CAIRO_FONT_TYPE_WIN32:  return new_FontFace(L, LUACAIRO ".Win32FontFace.mt", ff, typeff, haveff);
#endif
#if CAIRO_HAS_QUARTZ_FONT
    case CAIRO_FONT_TYPE_QUARTZ: return new_FontFace(L, LUACAIRO ".QuartzFontFace.mt", ff, typeff, haveff);
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    case CAIRO_FONT_TYPE_USER:   return new_FontFace(L, LUACAIRO ".UserFontFace.mt", ff, typeff, haveff);
#endif
    }

    return 0;
}

static int gc_FontFace (lua_State *L)
{
    FontFace *o = check_FontFace_ud(L, 1);

    if (o->ff_ && o->haveff_)
    {
        cairo_font_face_destroy (o->ff_);
        o->ff_     = NULL;
        o->haveff_ = 0;
        o->typeff_ = 0;
    }

    return 0;
}

static int tostring_FontFace (lua_State *L)
{
    FontFace *o = check_FontFace_ud(L, 1);
    lua_pushfstring(L, "FontFace (%p), cairo_font_face_t (%p), Type:%d", (void*)o, (void*)o->ff_, o->typeff_);
    return 1;
}



static int luaopen_lfontface (lua_State *L)
{
    return 0;
}


#endif  /* LUACAIRO_HAS_OO_INTERFACE */




static int luaopen_lcairo_ud_font_face(lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lfontface (L);

#endif /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
