/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


// #if CAIRO_HAS_RECORDING_SURFACE



/* Recording-surface functions */

// cairo_public cairo_surface_t *
// cairo_recording_surface_create (cairo_content_t		 content,
//                                 const cairo_rectangle_t *extents);
static int l_cairo_recording_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_recording_surface_ink_extents (cairo_surface_t *surface,
//                                      double *x0,
//                                      double *y0,
//                                      double *width,
//                                      double *height);
static int l_cairo_recording_surface_ink_extents (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}



static const struct luaL_Reg recording_f [] = {
    {"recording_surface_create",                       l_cairo_recording_surface_create},
    {"recording_surface_ink_extents",                  l_cairo_recording_surface_ink_extents},
	{NULL, NULL} /* sentinel */
};


// #endif /* CAIRO_HAS_RECORDING_SURFACE */



static int luaopen_lcairo_recording_surface (lua_State* L)
{
// #if CAIRO_HAS_RECORDING_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, recording_f);

// #endif /* CAIRO_HAS_RECORDING_SURFACE */
    return 0;
}
