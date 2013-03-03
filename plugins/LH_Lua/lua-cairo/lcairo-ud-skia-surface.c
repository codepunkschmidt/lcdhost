/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// SkiaSurface userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_HAS_SKIA_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE



#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_SKIA_SURFACE */



static int luaopen_lcairo_ud_skia_surface (lua_State* L)
{
#if CAIRO_HAS_SKIA_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_SKIA_SURFACE */
    return 0;
}


