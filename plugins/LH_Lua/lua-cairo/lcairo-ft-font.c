/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_FT_FONT
#include <cairo-ft.h>



/* Fontconfig/Freetype platform-specific font interface */


static int l_cairo_ft_font_face_create_for_pattern (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static int l_cairo_ft_font_options_substitute (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public cairo_font_face_t *
// cairo_ft_font_face_create_for_ft_face (FT_Face         face,
//                        int             load_flags);
static int l_cairo_ft_font_face_create_for_ft_face(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public FT_Face
// cairo_ft_scaled_font_lock_face (cairo_scaled_font_t *scaled_font);
static int l_cairo_ft_scaled_font_lock_face(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_ft_scaled_font_unlock_face (cairo_scaled_font_t *scaled_font);
static int l_cairo_ft_scaled_font_unlock_face(lua_State* L)
{
    //cairo_scaled_font_t *scaled_font = get_cairo_scaled_font_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}


#if CAIRO_HAS_FC_FONT

// cairo_public cairo_font_face_t *
// cairo_ft_font_face_create_for_pattern (FcPattern *pattern);
static int l_cairo_ft_font_face_create_for_pattern(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_ft_font_options_substitute (const cairo_font_options_t *options,
//                   FcPattern                  *pattern);
static int l_cairo_ft_font_options_substitute(lua_State* L)
{
    //const cairo_font_options_t *options = get_cairo_font_options_t (L, 1);
    luaL_error(L, "not implemented!");
    return 0;
}

#endif /* CAIRO_HAS_FC_FONT */



static const struct luaL_Reg ft_font_f [] = {
    {"ft_font_face_create_for_pattern",          l_cairo_ft_font_face_create_for_pattern},
    {"ft_font_options_substitute",               l_cairo_ft_font_options_substitute},
    {"ft_font_face_create_for_ft_face",          l_cairo_ft_font_face_create_for_ft_face},
    {"ft_scaled_font_lock_face",                 l_cairo_ft_scaled_font_lock_face},
    {"ft_scaled_font_unlock_face",               l_cairo_ft_scaled_font_unlock_face},
#if CAIRO_HAS_FC_FONT
    {"ft_font_face_create_for_pattern",          l_cairo_ft_font_face_create_for_pattern},
    {"ft_font_options_substitute",               l_cairo_ft_font_options_substitute},
#endif /* CAIRO_HAS_FC_FONT */
	{NULL, NULL} /* sentinel */
};

#endif /* CAIRO_HAS_FT_FONT */



static int luaopen_lcairo_ft_font (lua_State* L)
{
#if CAIRO_HAS_FT_FONT

    // register functions
    luaL_register(L, LUACAIRO, ft_font_f);

#endif /* CAIRO_HAS_FT_FONT */

    return 0;
}
