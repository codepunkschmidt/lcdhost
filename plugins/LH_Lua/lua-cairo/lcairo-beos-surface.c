/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_BEOS_SURFACE
#include <cairo-beos.h>



// cairo_public cairo_surface_t *
// cairo_beos_surface_create (BView* view);
static int l_cairo_beos_surface_create(lua_State* L)
{
    BView* view = (BView *) check_lightuserdata(L, 1);
    cairo_surface_t *v = cairo_beos_surface_create (view);
    lua_pushlightuserdata(L, v);
    return 1;
}


// cairo_public cairo_surface_t *
// cairo_beos_surface_create_for_bitmap (BView*   view,
//                       BBitmap* bmp);
static int l_cairo_beos_surface_create_for_bitmap(lua_State* L)
{
    BView* view = (BView *) check_lightuserdata(L, 1);
    BBitmap* bmp = (BBitmap *) check_lightuserdata(L, 2);
    cairo_surface_t *v = cairo_beos_surface_create_for_bitmap (view, bmp);
    lua_pushlightuserdata(L, v);
    return 1;
}


static const struct luaL_Reg beos_surface_f [] = {
    {"beos_surface_create",                      l_cairo_beos_surface_create},
    {"beos_surface_create_for_bitmap",           l_cairo_beos_surface_create_for_bitmap},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_BEOS_SURFACE */



static int luaopen_lcairo_beos_surface(lua_State* L)
{
#if CAIRO_HAS_BEOS_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, beos_surface_f);

#endif /* CAIRO_HAS_BEOS_SURFACE */
    return 0;
}
