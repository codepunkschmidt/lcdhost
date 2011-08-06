/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_XML_SURFACE
#include <cairo-xml.h>



// cairo_public cairo_device_t *
// cairo_xml_create (const char *filename);
static int l_cairo_xml_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_device_t *
// cairo_xml_create_for_stream (cairo_write_func_t	 write_func,
// 			     void		*closure);
static int l_cairo_xml_create_for_stream (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_xml_surface_create (cairo_device_t *xml,
// 			  cairo_content_t content,
// 			  double width, double height);
static int l_cairo_xml_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_status_t
// cairo_xml_for_recording_surface (cairo_device_t *xml,
// 				 cairo_surface_t *surface);
static int l_cairo_xml_for_recording_surface (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg xml_f [] = {
    {"xml_create",                       l_cairo_xml_create},
    {"xml_create_for_stream",            l_cairo_xml_create_for_stream},
    {"xml_surface_create",               l_cairo_xml_surface_create},
    {"xml_for_recording_surface",        l_cairo_xml_for_recording_surface},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_XML_SURFACE */



static int luaopen_lcairo_xml_surface (lua_State* L)
{
#if CAIRO_HAS_XML_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, xml_f);

#endif /* CAIRO_HAS_XML_SURFACE */

    return 0;
}
