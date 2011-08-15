/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif


#if CAIRO_HAS_VG_SURFACE
#include <cairo-vg.h>



#if CAIRO_HAS_GLX_FUNCTIONS
// cairo_public cairo_vg_context_t *
// cairo_vg_context_create_for_glx (Display *dpy,
// 				 GLXContext ctx);
static int l_cairo_vg_context_create_for_glx (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif

#if CAIRO_HAS_EGL_FUNCTIONS
// cairo_public cairo_vg_context_t *
// cairo_vg_context_create_for_egl (EGLDisplay egl_display,
// 				 EGLContext egl_context);
static int l_cairo_vg_context_create_for_egl (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}
#endif

// cairo_public cairo_status_t
// cairo_vg_context_status (cairo_vg_context_t *context);
static int l_cairo_vg_context_status (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_vg_context_destroy (cairo_vg_context_t *context);
static int l_cairo_vg_context_destroy (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_vg_surface_create (cairo_vg_context_t *context,
// 			 cairo_content_t content, int width, int height);
static int l_cairo_vg_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_vg_surface_create_for_image (cairo_vg_context_t *context,
// 				   VGImage image,
// 				   VGImageFormat format,
// 				   int width, int height);
static int l_cairo_vg_surface_create_for_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public VGImage
// cairo_vg_surface_get_image (cairo_surface_t *abstract_surface);
static int l_cairo_vg_surface_get_image (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public VGImageFormat
// cairo_vg_surface_get_format (cairo_surface_t *abstract_surface);
static int l_cairo_vg_surface_get_format (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_vg_surface_get_height (cairo_surface_t *abstract_surface);
static int l_cairo_vg_surface_get_height (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_vg_surface_get_width (cairo_surface_t *abstract_surface);
static int l_cairo_vg_surface_get_width (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}


static const struct luaL_Reg vg_f [] = {
#if CAIRO_HAS_GLX_FUNCTIONS
    {"vg_context_create_for_glx",                       l_cairo_vg_context_create_for_glx},
#endif
#if CAIRO_HAS_EGL_FUNCTIONS
    {"vg_context_create_for_egl",                       l_cairo_vg_context_create_for_egl},
#endif
    {"vg_context_status",                               l_cairo_vg_context_status},
    {"vg_context_destroy",                              l_cairo_vg_context_destroy},
    {"vg_surface_create",                               l_cairo_vg_surface_create},
    {"vg_surface_create_for_image",                     l_cairo_vg_surface_create_for_image},
    {"vg_surface_get_image",                            l_cairo_vg_surface_get_image},
    {"vg_surface_get_format",                           l_cairo_vg_surface_get_format},
    {"vg_surface_get_height",                           l_cairo_vg_surface_get_height},
    {"vg_surface_get_width",                            l_cairo_vg_surface_get_width},
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_VG_SURFACE */



static int luaopen_lcairo_vg_surface (lua_State* L)
{
#if CAIRO_HAS_VG_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, vg_f);

#endif /* CAIRO_HAS_VG_SURFACE */
    return 0;
}
