/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



#if CAIRO_HAS_GL_SURFACE
#include <cairo-gl.h>


// cairo_public cairo_surface_t *
// cairo_gl_surface_create (cairo_device_t *device,
// 			 cairo_content_t content,
// 			 int width, int height);
static int l_cairo_gl_surface_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_gl_surface_create_for_texture (cairo_device_t *abstract_device,
// 				     cairo_content_t content,
// 				     unsigned int tex,
//                                      int width, int height);
static int l_cairo_gl_surface_create_for_texture (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_gl_surface_set_size (cairo_surface_t *surface, int width, int height);
static int l_cairo_gl_surface_set_size (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_gl_surface_get_width (cairo_surface_t *abstract_surface);
static int l_cairo_gl_surface_get_width (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public int
// cairo_gl_surface_get_height (cairo_surface_t *abstract_surface);
static int l_cairo_gl_surface_get_height (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public void
// cairo_gl_surface_swapbuffers (cairo_surface_t *surface);
static int l_cairo_gl_surface_swapbuffers (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

#if CAIRO_HAS_GLX_FUNCTIONS

// cairo_public cairo_device_t *
// cairo_glx_device_create (Display *dpy, GLXContext gl_ctx);
static int l_cairo_glx_device_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public Display *
// cairo_glx_device_get_display (cairo_device_t *device);
static int l_cairo_glx_device_get_display (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public GLXContext
// cairo_glx_device_get_context (cairo_device_t *device);
static int l_cairo_glx_device_get_context (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_gl_surface_create_for_window (cairo_device_t *device,
// 				    Window win,
// 				    int width, int height);
static int l_cairo_gl_surface_create_for_window (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

#endif

#if CAIRO_HAS_WGL_FUNCTIONS

// cairo_public cairo_device_t *
// cairo_wgl_device_create (HGLRC rc);
static int l_cairo_wgl_device_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public HGLRC
// cairo_wgl_device_get_context (cairo_device_t *device);
static int l_cairo_wgl_device_get_context (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_gl_surface_create_for_dc (cairo_device_t		*device,
// 				HDC			 dc,
// 				int			 width,
// 				int			 height);
static int l_cairo_gl_surface_create_for_dc (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

#endif

#if CAIRO_HAS_EGL_FUNCTIONS

// cairo_public cairo_device_t *
// cairo_egl_device_create (EGLDisplay dpy, EGLContext egl);
static int l_cairo_egl_device_create (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

// cairo_public cairo_surface_t *
// cairo_gl_surface_create_for_egl (cairo_device_t	*device,
// 				 EGLSurface	 egl,
// 				 int		 width,
// 				 int		 height);
static int l_cairo_gl_surface_create_for_egl (lua_State* L)
{
    luaL_error(L, "not implemented!");
    return 0;
}

#endif



static const struct luaL_Reg gl_surface_f [] = {
    {"gl_surface_create",                      l_cairo_gl_surface_create},
    {"gl_surface_create_for_texture",          l_cairo_gl_surface_create_for_texture},
    {"gl_surface_set_size",                    l_cairo_gl_surface_set_size},
    {"gl_surface_get_width",                   l_cairo_gl_surface_get_width},
    {"gl_surface_get_height",                  l_cairo_gl_surface_get_height},
    {"gl_surface_swapbuffers",                 l_cairo_gl_surface_swapbuffers},
#if CAIRO_HAS_GLX_FUNCTIONS
    {"glx_device_create",                      l_cairo_glx_device_create},
    {"glx_device_get_display",                 l_cairo_glx_device_get_display},
    {"glx_device_get_context",                 l_cairo_glx_device_get_context},
    {"gl_surface_create_for_window",           l_cairo_gl_surface_create_for_window},
#endif
#if CAIRO_HAS_WGL_FUNCTIONS
    {"wgl_device_create",                      l_cairo_wgl_device_create},
    {"wgl_device_get_context",                 l_cairo_wgl_device_get_context},
    {"gl_surface_create_for_dc",               l_cairo_gl_surface_create_for_dc},
#endif
#if CAIRO_HAS_EGL_FUNCTIONS
    {"egl_device_create",                      l_cairo_egl_device_create},
    {"gl_surface_create_for_egl",              l_cairo_gl_surface_create_for_egl},
#endif
	{NULL, NULL} /* sentinel */
};


#endif /* CAIRO_HAS_GL_SURFACE */



static int luaopen_lcairo_gl_surface (lua_State* L)
{
#if CAIRO_HAS_GL_SURFACE

    // register functions
    luaL_register(L, LUACAIRO, gl_surface_f);

#endif /* CAIRO_HAS_GL_SURFACE */

    return 0;
}
