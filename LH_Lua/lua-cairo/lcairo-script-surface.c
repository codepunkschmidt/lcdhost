/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_SCRIPT_SURFACE
#include <cairo-script.h>




// cairo_public cairo_device_t *
// cairo_script_create (const char *filename);
static int l_cairo_script_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_device_t *
// cairo_script_create_for_stream (cairo_write_func_t	 write_func,
// 				void			*closure);
static int l_cairo_script_create_for_stream (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_script_write_comment (cairo_device_t *script,
// 			    const char *comment,
// 			    int len);
static int l_cairo_script_write_comment (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_script_set_mode (cairo_device_t *script,
// 		       cairo_script_mode_t mode);
static int l_cairo_script_set_mode (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_script_mode_t
// cairo_script_get_mode (cairo_device_t *script);
static int l_cairo_script_get_mode (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_script_surface_create (cairo_device_t *script,
// 			     cairo_content_t content,
// 			     double width,
// 			     double height);
static int l_cairo_script_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_script_surface_create_for_target (cairo_device_t *script,
// 					cairo_surface_t *target);
static int l_cairo_script_surface_create_for_target (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_status_t
// cairo_script_from_recording_surface (cairo_device_t	*script,
// 				     cairo_surface_t	*recording_surface);
static int l_cairo_script_from_recording_surface (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg script_f [] = {
    {"script_create",                        l_cairo_script_create},
    {"script_create_for_stream",             l_cairo_script_create_for_stream},
    {"script_write_comment",                 l_cairo_script_write_comment},
    {"script_set_mode",                      l_cairo_script_set_mode},
    {"script_get_mode",                      l_cairo_script_get_mode},
    {"script_surface_create",                l_cairo_script_surface_create},
    {"script_surface_create_for_target",     l_cairo_script_surface_create_for_target},
    {"script_from_recording_surface",        l_cairo_script_from_recording_surface},
	{NULL, NULL} /* sentinel */
};


#endif /*CAIRO_HAS_SCRIPT_SURFACE*/




static int luaopen_lcairo_script_surface (lua_State* L)
{
#if CAIRO_HAS_SCRIPT_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, script_f);

#endif /*CAIRO_HAS_SCRIPT_SURFACE*/

    return 0;
}
