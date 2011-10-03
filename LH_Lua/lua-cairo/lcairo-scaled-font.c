/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




/* Portable interface to general font features. */

// cairo_public cairo_scaled_font_t *
// cairo_scaled_font_create (cairo_font_face_t *font_face,
//               const cairo_matrix_t *font_matrix,
//               const cairo_matrix_t *ctm,
//               const cairo_font_options_t *options);
static int l_cairo_scaled_font_create(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    const cairo_matrix_t *font_matrix = get_cairo_matrix_t (L, 2);
    const cairo_matrix_t *ctm = get_cairo_matrix_t (L, 3);
    const cairo_font_options_t *options = get_cairo_font_options_t (L, 4);
    cairo_scaled_font_t *v = cairo_scaled_font_create (font_face, font_matrix, ctm, options);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public cairo_scaled_font_t *
// cairo_scaled_font_reference (cairo_scaled_font_t *scaled_font);
static int l_cairo_scaled_font_reference(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_scaled_font_t *v = cairo_scaled_font_reference (scaled_font);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public void
// cairo_scaled_font_destroy (cairo_scaled_font_t *scaled_font);
static int l_cairo_scaled_font_destroy(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = (cairo_scaled_font_t *) check_lightuserdata(L, 1);
    cairo_scaled_font_destroy (scaled_font);
    return 0;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public unsigned int
// cairo_scaled_font_get_reference_count (cairo_scaled_font_t *scaled_font);
static int l_cairo_scaled_font_get_reference_count(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    unsigned int v = cairo_scaled_font_get_reference_count (scaled_font);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_scaled_font_status (cairo_scaled_font_t *scaled_font);
static int l_cairo_scaled_font_status(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_status_t v = cairo_scaled_font_status (scaled_font);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_font_type_t
// cairo_scaled_font_get_type (cairo_scaled_font_t *scaled_font);
static int l_cairo_scaled_font_get_type(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_font_type_t v = cairo_scaled_font_get_type (scaled_font);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void *
// cairo_scaled_font_get_user_data (cairo_scaled_font_t *scaled_font,
//                  const cairo_user_data_key_t *key);
static int l_cairo_scaled_font_get_user_data(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_status_t
// cairo_scaled_font_set_user_data (cairo_scaled_font_t *scaled_font,
//                  const cairo_user_data_key_t *key,
//                  void *user_data,
//                  cairo_destroy_func_t destroy);
static int l_cairo_scaled_font_set_user_data(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_scaled_font_extents (cairo_scaled_font_t *scaled_font,
//                cairo_font_extents_t *extents);
static int l_cairo_scaled_font_extents(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_font_extents_t *extents = get_cairo_font_extents_t (L, 2);
    cairo_scaled_font_extents (scaled_font, extents);
    return 0;
}


// cairo_public void
// cairo_scaled_font_text_extents (cairo_scaled_font_t *scaled_font,
//                 const char *utf8,
//                 cairo_text_extents_t *extents);
static int l_cairo_scaled_font_text_extents(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    const char *utf8 = luaL_checkstring(L, 2);
    cairo_text_extents_t *extents = get_cairo_text_extents_t (L, 3);
    cairo_scaled_font_text_extents (scaled_font, utf8, extents);
    return 0;
}


// cairo_public void
// cairo_scaled_font_glyph_extents (cairo_scaled_font_t *scaled_font,
//                  const cairo_glyph_t *glyphs,
//                  int num_glyphs,
//                  cairo_text_extents_t *extents);
static int l_cairo_scaled_font_glyph_extents(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public cairo_status_t
// cairo_scaled_font_text_to_glyphs (cairo_scaled_font_t *scaled_font,
//                   double x,
//                   double y,
//                   const char *utf8,
//                   int utf8_len,
//                   cairo_glyph_t **glyphs,
//                   int *num_glyphs,
//                   cairo_text_cluster_t **clusters,
//                   int *num_clusters,
//                   cairo_text_cluster_flags_t *cluster_flags);
static int l_cairo_scaled_font_text_to_glyphs(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


// cairo_public cairo_font_face_t *
// cairo_scaled_font_get_font_face (cairo_scaled_font_t *scaled_font);
static int l_cairo_scaled_font_get_font_face(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_font_face_t *v = cairo_scaled_font_get_font_face (scaled_font);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public void
// cairo_scaled_font_get_font_matrix (cairo_scaled_font_t *scaled_font,
//                    cairo_matrix_t *font_matrix);
static int l_cairo_scaled_font_get_font_matrix(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_matrix_t *font_matrix = get_cairo_matrix_t (L, 2);
    cairo_scaled_font_get_font_matrix (scaled_font, font_matrix);
    return 0;
}


// cairo_public void
// cairo_scaled_font_get_ctm (cairo_scaled_font_t *scaled_font,
//                cairo_matrix_t *ctm);
static int l_cairo_scaled_font_get_ctm(lua_State* L)
{
    cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_matrix_t *ctm = get_cairo_matrix_t (L, 2);
    cairo_scaled_font_get_ctm (scaled_font, ctm);
    return 0;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
// cairo_public void
// cairo_scaled_font_get_scale_matrix (cairo_scaled_font_t *scaled_font,
//                     cairo_matrix_t *scale_matrix);
static int l_cairo_scaled_font_get_scale_matrix(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


// cairo_public void
// cairo_scaled_font_get_font_options (cairo_scaled_font_t *scaled_font,
//                     cairo_font_options_t	*options);
static int l_cairo_scaled_font_get_font_options(lua_State* L)
{
    cairo_scaled_font_t	*scaled_font = get_cairo_scaled_font_t (L, 1);
    cairo_font_options_t *options = get_cairo_font_options_t (L, 2);
    cairo_scaled_font_get_font_options (scaled_font, options);
    return 0;
}




static const struct luaL_Reg scaled_font_f [] = {
    {"scaled_font_create",                       l_cairo_scaled_font_create},
    {"scaled_font_reference",                    l_cairo_scaled_font_reference},
    {"scaled_font_destroy",                      l_cairo_scaled_font_destroy},
    {"scaled_font_get_reference_count",          l_cairo_scaled_font_get_reference_count},
    {"scaled_font_status",                       l_cairo_scaled_font_status},
    {"scaled_font_get_type",                     l_cairo_scaled_font_get_type},
    {"scaled_font_get_user_data",                l_cairo_scaled_font_get_user_data},
    {"scaled_font_set_user_data",                l_cairo_scaled_font_set_user_data},
    {"scaled_font_extents",                      l_cairo_scaled_font_extents},
    {"scaled_font_text_extents",                 l_cairo_scaled_font_text_extents},
    {"scaled_font_glyph_extents",                l_cairo_scaled_font_glyph_extents},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"scaled_font_text_to_glyphs",               l_cairo_scaled_font_text_to_glyphs},
#endif
    {"scaled_font_get_font_face",                l_cairo_scaled_font_get_font_face},
    {"scaled_font_get_font_matrix",              l_cairo_scaled_font_get_font_matrix},
    {"scaled_font_get_ctm",                      l_cairo_scaled_font_get_ctm},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    {"scaled_font_get_scale_matrix",             l_cairo_scaled_font_get_scale_matrix},
#endif
    {"scaled_font_get_font_options",             l_cairo_scaled_font_get_font_options},
	{NULL, NULL} /* sentinel */
};



static int luaopen_lcairo_scaled_font(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, scaled_font_f);
    return 0;
}
