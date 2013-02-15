/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




/* User fonts */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)


// cairo_public cairo_font_face_t *
// cairo_user_font_face_create (void);
static int l_cairo_user_font_face_create(lua_State* L)
{
    cairo_font_face_t *v = cairo_user_font_face_create ();
    lua_pushlightuserdata(L, v);
    return 1;
}


/* User-font method signatures */

/* User-font method setters */

// cairo_public void
// cairo_user_font_face_set_init_func (cairo_font_face_t *font_face,
//                     cairo_user_scaled_font_init_func_t  init_func);
static int l_cairo_user_font_face_set_init_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_user_font_face_set_render_glyph_func (cairo_font_face_t *font_face,
//                         cairo_user_scaled_font_render_glyph_func_t  render_glyph_func);
static int l_cairo_user_font_face_set_render_glyph_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_user_font_face_set_text_to_glyphs_func (cairo_font_face_t *font_face,
//                           cairo_user_scaled_font_text_to_glyphs_func_t  text_to_glyphs_func);
static int l_cairo_user_font_face_set_text_to_glyphs_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_user_font_face_set_unicode_to_glyph_func (cairo_font_face_t *font_face,
//                             cairo_user_scaled_font_unicode_to_glyph_func_t  unicode_to_glyph_func);
static int l_cairo_user_font_face_set_unicode_to_glyph_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


/* User-font method getters */

// cairo_public cairo_user_scaled_font_init_func_t
// cairo_user_font_face_get_init_func (cairo_font_face_t *font_face);
static int l_cairo_user_font_face_get_init_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_user_scaled_font_render_glyph_func_t
// cairo_user_font_face_get_render_glyph_func (cairo_font_face_t *font_face);
static int l_cairo_user_font_face_get_render_glyph_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_user_scaled_font_text_to_glyphs_func_t
// cairo_user_font_face_get_text_to_glyphs_func (cairo_font_face_t *font_face);
static int l_cairo_user_font_face_get_text_to_glyphs_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_user_scaled_font_unicode_to_glyph_func_t
// cairo_user_font_face_get_unicode_to_glyph_func (cairo_font_face_t *font_face);
static int l_cairo_user_font_face_get_unicode_to_glyph_func(lua_State* L)
{
    //cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg user_font_face_f [] = {
    {"user_font_face_create",                    l_cairo_user_font_face_create},
    {"user_font_face_set_init_func",             l_cairo_user_font_face_set_init_func},
    {"user_font_face_set_render_glyph_func",     l_cairo_user_font_face_set_render_glyph_func},
    {"user_font_face_set_text_to_glyphs_func",   l_cairo_user_font_face_set_text_to_glyphs_func},
    {"user_font_face_set_unicode_to_glyph_func", l_cairo_user_font_face_set_unicode_to_glyph_func},
    {"user_font_face_get_init_func",             l_cairo_user_font_face_get_init_func},
    {"user_font_face_get_render_glyph_func",     l_cairo_user_font_face_get_render_glyph_func},
    {"user_font_face_get_text_to_glyphs_func",   l_cairo_user_font_face_get_text_to_glyphs_func},
    {"user_font_face_get_unicode_to_glyph_func", l_cairo_user_font_face_get_unicode_to_glyph_func},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */


static int luaopen_lcairo_user_font_face(lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)

    // register functions
    luaL_register(L, LUACAIRO, user_font_face_f);
    
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */
    return 0;
}
