/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



/* Toy fonts */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)


// cairo_public cairo_font_face_t *
// cairo_toy_font_face_create (const char *family,
//                 cairo_font_slant_t slant,
//                 cairo_font_weight_t weight);
static int l_cairo_toy_font_face_create(lua_State* L)
{
    const char *family = luaL_checkstring(L, 1);
    cairo_font_slant_t slant = (cairo_font_slant_t) luaL_checkinteger(L, 2);
    cairo_font_weight_t weight = (cairo_font_weight_t) luaL_checkinteger(L, 3);
    cairo_font_face_t *v = cairo_toy_font_face_create (family, slant, weight);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public const char *
// cairo_toy_font_face_get_family (cairo_font_face_t *font_face);
static int l_cairo_toy_font_face_get_family(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    const char *v = cairo_toy_font_face_get_family (font_face);
    lua_pushstring(L, v);
    return 1;
}


// cairo_public cairo_font_slant_t
// cairo_toy_font_face_get_slant (cairo_font_face_t *font_face);
static int l_cairo_toy_font_face_get_slant(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    cairo_font_slant_t v = cairo_toy_font_face_get_slant (font_face);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_font_weight_t
// cairo_toy_font_face_get_weight (cairo_font_face_t *font_face);
static int l_cairo_toy_font_face_get_weight(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    cairo_font_weight_t v = cairo_toy_font_face_get_weight (font_face);
    lua_pushinteger(L, v);
    return 1;
}



static const struct luaL_Reg toy_font_face_f [] = {
    {"toy_font_face_create",                     l_cairo_toy_font_face_create},
    {"toy_font_face_get_family",                 l_cairo_toy_font_face_get_family},
    {"toy_font_face_get_slant",                  l_cairo_toy_font_face_get_slant},
    {"toy_font_face_get_weight",                 l_cairo_toy_font_face_get_weight},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */


static int luaopen_lcairo_toy_font_face(lua_State* L)
{
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)

    // register functions
    luaL_register(L, LUACAIRO, toy_font_face_f);

#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */
    return 0;
}
