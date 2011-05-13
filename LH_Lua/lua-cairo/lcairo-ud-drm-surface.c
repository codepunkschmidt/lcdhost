/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// DrmSurface userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_HAS_DRM_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE



#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_DRM_SURFACE */



static int luaopen_lcairo_ud_drm_surface (lua_State* L)
{
#if CAIRO_HAS_DRM_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE

#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_DRM_SURFACE */
    return 0;
}


