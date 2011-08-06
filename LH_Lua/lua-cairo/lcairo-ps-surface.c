/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_PS_SURFACE
#include <cairo-ps.h>



/* PS-surface functions */

// cairo_public cairo_surface_t *
// cairo_ps_surface_create (const char		*filename,
//              double			 width_in_points,
//              double			 height_in_points);
static int l_cairo_ps_surface_create(lua_State* L)
{
    const char *filename = luaL_checkstring(L, 1);
    double width_in_points = luaL_checknumber(L, 2);
    double height_in_points = luaL_checknumber(L, 3);
    cairo_surface_t *v = cairo_ps_surface_create(filename, width_in_points, height_in_points);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_ps_surface_create_for_stream (cairo_write_func_t	write_func,
//                     void	       *closure,
//                     double		width_in_points,
//                     double		height_in_points);
static int l_cairo_ps_surface_create_for_stream(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public void
// cairo_ps_surface_restrict_to_level (cairo_surface_t    *surface,
//                                     cairo_ps_level_t    level);
static int l_cairo_ps_surface_restrict_to_level(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    cairo_ps_level_t level = (cairo_ps_level_t)luaL_checkinteger(L, 2);
    cairo_ps_surface_restrict_to_level (surface, level);
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public void
// cairo_ps_get_levels (cairo_ps_level_t const  **levels,
//                      int                      *num_levels);
static int l_cairo_ps_get_levels(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public const char *
// cairo_ps_level_to_string (cairo_ps_level_t level);
static int l_cairo_ps_level_to_string(lua_State* L)
{
    cairo_ps_level_t level = (cairo_ps_level_t)luaL_checkinteger(L, 1);
    const char *v = cairo_ps_level_to_string (level);
    lua_pushstring(L, v);
    return 1;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public void
// cairo_ps_surface_set_eps (cairo_surface_t	*surface,
//               cairo_bool_t           eps);
static int l_cairo_ps_surface_set_eps(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    cairo_bool_t eps = (cairo_bool_t)luaL_checkinteger(L, 2);
    cairo_ps_surface_set_eps (surface, eps);
    return 0;
}
#endif


#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
// cairo_public cairo_bool_t
// cairo_ps_surface_get_eps (cairo_surface_t	*surface);
static int l_cairo_ps_surface_get_eps(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    cairo_bool_t v = cairo_ps_surface_get_eps (surface);
    lua_pushboolean(L, v);
    return 1;
}
#endif


// cairo_public void
// cairo_ps_surface_set_size (cairo_surface_t	*surface,
//                double		 width_in_points,
//                double		 height_in_points);
static int l_cairo_ps_surface_set_size(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    double width_in_points = luaL_checknumber(L, 2);
    double height_in_points = luaL_checknumber(L, 3);
    cairo_ps_surface_set_size (surface, width_in_points, height_in_points);
    return 0;
}


// cairo_public void
// cairo_ps_surface_dsc_comment (cairo_surface_t	*surface,
//                   const char	*comment);
static int l_cairo_ps_surface_dsc_comment(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    const char *comment = luaL_checkstring(L, 2);
    cairo_ps_surface_dsc_comment (surface, comment);
    return 0;
}


// cairo_public void
// cairo_ps_surface_dsc_begin_setup (cairo_surface_t *surface);
static int l_cairo_ps_surface_dsc_begin_setup(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    cairo_ps_surface_dsc_begin_setup (surface);
    return 0;
}


// cairo_public void
// cairo_ps_surface_dsc_begin_page_setup (cairo_surface_t *surface);
static int l_cairo_ps_surface_dsc_begin_page_setup(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    cairo_ps_surface_dsc_begin_page_setup (surface);
    return 0;
}


static const struct luaL_Reg ps_f [] = {
    {"ps_surface_create",                        l_cairo_ps_surface_create},
    {"ps_surface_create_for_stream",             l_cairo_ps_surface_create_for_stream},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"ps_surface_restrict_to_level",             l_cairo_ps_surface_restrict_to_level},
    {"ps_get_levels",                            l_cairo_ps_get_levels},
    {"ps_level_to_string",                       l_cairo_ps_level_to_string},
    {"ps_surface_set_eps",                       l_cairo_ps_surface_set_eps},
    {"ps_surface_get_eps",                       l_cairo_ps_surface_get_eps},
#endif
    {"ps_surface_set_size",                      l_cairo_ps_surface_set_size},
    {"ps_surface_dsc_comment",                   l_cairo_ps_surface_dsc_comment},
    {"ps_surface_dsc_begin_setup",               l_cairo_ps_surface_dsc_begin_setup},
    {"ps_surface_dsc_begin_page_setup",          l_cairo_ps_surface_dsc_begin_page_setup},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_PS_SURFACE */



static int luaopen_lcairo_ps_surface (lua_State* L)
{
#if CAIRO_HAS_PS_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, ps_f);

#endif /* CAIRO_HAS_PS_SURFACE */

    return 0;
}
