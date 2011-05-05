/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_TEE_SURFACE
#include <cairo-tee.h>



// cairo_public cairo_surface_t *
// cairo_tee_surface_create (cairo_surface_t *master);
static int l_cairo_tee_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_tee_surface_add (cairo_surface_t *surface,
// 		       cairo_surface_t *target);
static int l_cairo_tee_surface_add (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_tee_surface_remove (cairo_surface_t *surface,
// 			  cairo_surface_t *target);
static int l_cairo_tee_surface_remove (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_tee_surface_index (cairo_surface_t *surface,
// 			 int index);
static int l_cairo_tee_surface_index (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg tee_f [] = {
    {"tee_surface_create",                       l_cairo_tee_surface_create},
    {"tee_surface_add",                          l_cairo_tee_surface_add},
    {"tee_surface_remove",                       l_cairo_tee_surface_remove},
    {"tee_surface_index",                        l_cairo_tee_surface_index},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_TEE_SURFACE */



static int luaopen_lcairo_tee_surface (lua_State* L)
{
#if CAIRO_HAS_TEE_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, tee_f);

#endif /* CAIRO_HAS_TEE_SURFACE */

    return 0;
}
