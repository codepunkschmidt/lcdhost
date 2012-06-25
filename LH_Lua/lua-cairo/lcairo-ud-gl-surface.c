/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif




//---------------------------------------------------------------------------
// GlSurface userdata for oo usage
//---------------------------------------------------------------------------



#if CAIRO_HAS_GL_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE


#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_GL_SURFACE */



static int luaopen_lcairo_ud_gl_surface (lua_State* L)
{
#if CAIRO_HAS_GL_SURFACE
#if LUACAIRO_HAS_OO_INTERFACE


#endif /* LUACAIRO_HAS_OO_INTERFACE */
#endif /* CAIRO_HAS_GL_SURFACE */
    return 0;
}
