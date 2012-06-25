/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_PDF_SURFACE
#include <cairo-pdf.h>


// cairo_public cairo_surface_t *
// cairo_pdf_surface_create (const char		*filename,
//               double		 width_in_points,
//               double		 height_in_points);
static int l_cairo_pdf_surface_create(lua_State* L)
{
    const char *filename = luaL_checkstring(L, 1);
    double width_in_points = luaL_checknumber(L, 2);
    double height_in_points = luaL_checknumber(L, 3);
    cairo_surface_t *v = cairo_pdf_surface_create(filename, width_in_points, height_in_points);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_pdf_surface_create_for_stream (cairo_write_func_t	write_func,
//                      void	       *closure,
//                      double		width_in_points,
//                      double		height_in_points);
static int l_cairo_pdf_surface_create_for_stream(lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_pdf_surface_set_size (cairo_surface_t	*surface,
//                 double		 width_in_points,
//                 double		 height_in_points);
static int l_cairo_pdf_surface_set_size(lua_State* L)
{
    cairo_surface_t	*surface = get_cairo_surface_t (L, 1);
    double width_in_points = luaL_checknumber(L, 2);
    double height_in_points = luaL_checknumber(L, 3);
    cairo_pdf_surface_set_size (surface, width_in_points, height_in_points);
    return 0;
}


// cairo_public void
// cairo_pdf_surface_restrict_to_version (cairo_surface_t 		*surface,
// 				       cairo_pdf_version_t  	 version);
static int l_cairo_pdf_surface_restrict_to_version (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public void
// cairo_pdf_get_versions (cairo_pdf_version_t const	**versions,
//                         int                      	 *num_versions);
static int l_cairo_pdf_get_versions (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


// cairo_public const char *
// cairo_pdf_version_to_string (cairo_pdf_version_t version);
static int l_cairo_pdf_version_to_string (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg pdf_f [] = {
    {"pdf_surface_create",                       l_cairo_pdf_surface_create},
    {"pdf_surface_create_for_stream",            l_cairo_pdf_surface_create_for_stream},
    {"pdf_surface_set_size",                     l_cairo_pdf_surface_set_size},
    {"pdf_surface_restrict_to_version",          l_cairo_pdf_surface_restrict_to_version},
    {"pdf_get_versions",                         l_cairo_pdf_get_versions},
    {"pdf_version_to_string",                    l_cairo_pdf_version_to_string},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_PDF_SURFACE */



static int luaopen_lcairo_pdf_surface (lua_State* L)
{
#if CAIRO_HAS_PDF_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, pdf_f);

#endif /* CAIRO_HAS_PDF_SURFACE */

    return 0;
}
