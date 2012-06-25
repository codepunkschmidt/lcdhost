/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




/* Generic identifier for a font style */

//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public cairo_font_face_t *
// cairo_font_face_reference (cairo_font_face_t *font_face);
static int l_cairo_font_face_reference(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    cairo_font_face_t *v = cairo_font_face_reference (font_face);
    lua_pushlightuserdata(L, v);
    return 1;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public void
// cairo_font_face_destroy (cairo_font_face_t *font_face);
static int l_cairo_font_face_destroy(lua_State* L)
{
    cairo_font_face_t *font_face = (cairo_font_face_t *) check_lightuserdata(L, 1);
    cairo_font_face_destroy (font_face);
    return 0;
}


//FIXME should not be exposed to the user of the language binding,
//FIXME but rather used to implement memory management within the language binding
// cairo_public unsigned int
// cairo_font_face_get_reference_count (cairo_font_face_t *font_face);
static int l_cairo_font_face_get_reference_count(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    unsigned int v = cairo_font_face_get_reference_count (font_face);
    lua_pushnumber(L, v);
    return 1;
}


// cairo_public cairo_status_t
// cairo_font_face_status (cairo_font_face_t *font_face);
static int l_cairo_font_face_status(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    cairo_status_t v = cairo_font_face_status (font_face);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public cairo_font_type_t
// cairo_font_face_get_type (cairo_font_face_t *font_face);
static int l_cairo_font_face_get_type(lua_State* L)
{
    cairo_font_face_t *font_face = get_cairo_font_face_t (L, 1);
    cairo_font_type_t v = cairo_font_face_get_type (font_face);
    lua_pushinteger(L, v);
    return 1;
}


// cairo_public void *
// cairo_font_face_get_user_data (cairo_font_face_t *font_face,
//                    const cairo_user_data_key_t *key);
static int l_cairo_font_face_get_user_data(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_status_t
// cairo_font_face_set_user_data (cairo_font_face_t *font_face,
//                    const cairo_user_data_key_t *key,
//                    void *user_data,
//                    cairo_destroy_func_t destroy);
static int l_cairo_font_face_set_user_data(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg font_face_f [] = {
    {"font_face_reference",                      l_cairo_font_face_reference},
    {"font_face_destroy",                        l_cairo_font_face_destroy},
    {"font_face_get_reference_count",            l_cairo_font_face_get_reference_count},
    {"font_face_status",                         l_cairo_font_face_status},
    {"font_face_get_type",                       l_cairo_font_face_get_type},
    {"font_face_get_user_data",                  l_cairo_font_face_get_user_data},
    {"font_face_set_user_data",                  l_cairo_font_face_set_user_data},
	{NULL, NULL} /* sentinel */
};



static int luaopen_lcairo_font_face(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, font_face_f);
    return 0;
}
