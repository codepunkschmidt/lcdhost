/* -*- C -*-
 *
 * Name:        lcairo.c
 * Purpose:     Lua binding of Cairo
 * Author:      Hakki Dogusan (dogusanh@tr.net)
 * Created:     2007-05-25
 * Copyright:   (c) 2007-2011 Hakki Dogusan
 * License:     MIT (http://www.opensource.org/licenses/mit-license.html)
 * RCS-ID:      $Id$
 * History:     2008-04-12 1.6.4.0  updated to Cairo-1.6.4
 *              2008-11-15 1.8.4.0  updated to Cairo-1.8.4
 *              2008-11-25 1.8.4.10 oo interface added
 *              2010-09-15 1.10.0.0 updated to Cairo-1.10.0
 *              2010-10-22 1.10.0.1 cairo_path_data_t related functions added
 *              2011-01-11 1.10.2.0 updated to Cairo-1.10.2
 *
 */

#include <string.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <cairo.h>


#define LUACAIRO "cairo"


#ifndef LUACAIRO_HAS_OO_INTERFACE
    #define LUACAIRO_HAS_OO_INTERFACE 1
#endif


// taken from cairo-image-surface.c
#ifndef CAIRO_HAS_IMAGE_SURFACE
    /**
     * CAIRO_HAS_IMAGE_SURFACE:
     *
     * Defined if the image surface backend is available.
     * The image surface backend is always built in.
     * This macro was added for completeness in cairo 1.8.
     *
     * @Since: 1.8
     */
     #define CAIRO_HAS_IMAGE_SURFACE 1
#endif


#ifndef CAIRO_HAS_PNG_FUNCTIONS
    #define CAIRO_HAS_PNG_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_TEST_SURFACES
    #define CAIRO_HAS_TEST_SURFACES 0
#endif
#ifndef CAIRO_HAS_USER_FONT
    #define CAIRO_HAS_USER_FONT 0
#endif
#ifndef CAIRO_HAS_FT_FONT
    #define CAIRO_HAS_FT_FONT 0
#endif
#ifndef CAIRO_HAS_FC_FONT
    #define CAIRO_HAS_FC_FONT 0
#endif
#ifndef CAIRO_HAS_BEOS_SURFACE
    #define CAIRO_HAS_BEOS_SURFACE 0
#endif
#ifndef CAIRO_HAS_DIRECTFB_SURFACE
    #define CAIRO_HAS_DIRECTFB_SURFACE 0
#endif
#ifndef CAIRO_HAS_DRM_SURFACE
    #define CAIRO_HAS_DRM_SURFACE 0
#endif
#ifndef CAIRO_HAS_DRM_XR_FUNCTIONS
    #define CAIRO_HAS_DRM_XR_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_GL_SURFACE
    #define CAIRO_HAS_GL_SURFACE 0
#endif
#ifndef CAIRO_HAS_GLX_FUNCTIONS
    #define CAIRO_HAS_GLX_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_WGL_FUNCTIONS
    #define CAIRO_HAS_WGL_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_EGL_FUNCTIONS
    #define CAIRO_HAS_EGL_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_OS2_SURFACE
    #define CAIRO_HAS_OS2_SURFACE 0
#endif
#ifndef CAIRO_HAS_PDF_SURFACE
    #define CAIRO_HAS_PDF_SURFACE 0
#endif
#ifndef CAIRO_HAS_PS_SURFACE
    #define CAIRO_HAS_PS_SURFACE 0
#endif
#ifndef CAIRO_HAS_QT_SURFACE
    #define CAIRO_HAS_QT_SURFACE 0
#endif
#ifndef CAIRO_HAS_QUARTZ_SURFACE
    #define CAIRO_HAS_QUARTZ_SURFACE 0
#endif
#ifndef CAIRO_HAS_QUARTZ_FONT
    #define CAIRO_HAS_QUARTZ_FONT 0
#endif
#ifndef CAIRO_HAS_QUARTZ_IMAGE_SURFACE
    #define CAIRO_HAS_QUARTZ_IMAGE_SURFACE 0
#endif
#ifndef CAIRO_HAS_SCRIPT_SURFACE
    #define CAIRO_HAS_SCRIPT_SURFACE 0
#endif
#ifndef CAIRO_HAS_SKIA_SURFACE
    #define CAIRO_HAS_SKIA_SURFACE 0
#endif
#ifndef CAIRO_HAS_SVG_SURFACE
    #define CAIRO_HAS_SVG_SURFACE 0
#endif
#ifndef CAIRO_HAS_TEE_SURFACE
    #define CAIRO_HAS_TEE_SURFACE 0
#endif
#ifndef CAIRO_HAS_VG_SURFACE
    #define CAIRO_HAS_VG_SURFACE 0
#endif
#ifndef CAIRO_HAS_WIN32_SURFACE
    #define CAIRO_HAS_WIN32_SURFACE 0
#endif
#ifndef CAIRO_HAS_WIN32_FONT
    #define CAIRO_HAS_WIN32_FONT 0
#endif
#ifndef CAIRO_HAS_XCB_SURFACE
    #define CAIRO_HAS_XCB_SURFACE 0
#endif
#ifndef CAIRO_HAS_XCB_DRM_FUNCTIONS
    #define CAIRO_HAS_XCB_DRM_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_XCB_SHM_FUNCTIONS
    #define CAIRO_HAS_XCB_SHM_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_XLIB_SURFACE
    #define CAIRO_HAS_XLIB_SURFACE 0
#endif
#ifndef CAIRO_HAS_XLIB_XRENDER_SURFACE
    #define CAIRO_HAS_XLIB_XRENDER_SURFACE 0
#endif
#ifndef CAIRO_HAS_XLIB_XCB_FUNCTIONS
    #define CAIRO_HAS_XLIB_XCB_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_XML_SURFACE
    #define CAIRO_HAS_XML_SURFACE 0
#endif
#ifndef CAIRO_HAS_GLITZ_SURFACE
    #define CAIRO_HAS_GLITZ_SURFACE 0
#endif
#ifndef CAIRO_HAS_RECORDING_SURFACE
    #define CAIRO_HAS_RECORDING_SURFACE 0
#endif
#ifndef CAIRO_HAS_GALLIUM_SURFACE
    #define CAIRO_HAS_GALLIUM_SURFACE 0
#endif
#ifndef CAIRO_HAS_SYMBOL_LOOKUP
    #define CAIRO_HAS_SYMBOL_LOOKUP 0
#endif
#ifndef CAIRO_HAS_PTHREAD
    #define CAIRO_HAS_PTHREAD 0
#endif
#ifndef CAIRO_HAS_GOBJECT_FUNCTIONS
    #define CAIRO_HAS_GOBJECT_FUNCTIONS 0
#endif
#ifndef CAIRO_HAS_TRACE
    #define CAIRO_HAS_TRACE 0
#endif
#ifndef CAIRO_HAS_INTERPRETER
    #define CAIRO_HAS_INTERPRETER 0
#endif



// --------------------------------------------------------------------------
//  binding includes
// --------------------------------------------------------------------------

#include "lcairo-private.h"
#include "lcairo-private.c"

#include "lcairo-beos-surface.c"
#include "lcairo-context.c"
#include "lcairo-device.c"
#include "lcairo-directfb-surface.c"
#include "lcairo-drm-surface.c"
#include "lcairo-drm-xr.c"
#include "lcairo-font-face.c"
#include "lcairo-font-options.c"
#include "lcairo-ft-font.c"
#include "lcairo-gl-surface.c"
#include "lcairo-glitz-surface.c"
#include "lcairo-image-surface.c"
#include "lcairo-matrix.c"
#include "lcairo-os2-surface.c"
#include "lcairo-path.c"
#include "lcairo-pattern.c"
#include "lcairo-pdf-surface.c"
#include "lcairo-ps-surface.c"
#include "lcairo-qt-surface.c"
#include "lcairo-quartz-font-face.c"
#include "lcairo-quartz-image-surface.c"
#include "lcairo-quartz-surface.c"
#include "lcairo-recording-surface.c"
#include "lcairo-region.c"
#include "lcairo-scaled-font.c"
#include "lcairo-script-surface.c"
#include "lcairo-skia-surface.c"
#include "lcairo-surface.c"
#include "lcairo-svg-surface.c"
#include "lcairo-tee-surface.c"
#include "lcairo-toy-font-face.c"
#include "lcairo-user-font-face.c"
#include "lcairo-vg-surface.c"
#include "lcairo-win32-font-face.c"
#include "lcairo-win32-scaled-font.c"
#include "lcairo-win32-surface.c"
#include "lcairo-xcb-surface.c"
#include "lcairo-xlib-surface.c"
#include "lcairo-xml-surface.c"

#include "lcairo-ud-font-extents.c"
#include "lcairo-ud-matrix.c"
#include "lcairo-ud-rectangle-int.c"
#include "lcairo-ud-text-extents.c"

#if LUACAIRO_HAS_OO_INTERFACE
    #include "lcairo-ud-pattern.c"
    #include "lcairo-ud-surface.c"
    #include "lcairo-ud-scaled-font.c"

    #include "lcairo-ud-beos-surface.c"
    #include "lcairo-ud-context.c"
    #include "lcairo-ud-device.c"
    #include "lcairo-ud-directfb-surface.c"
    #include "lcairo-ud-drm-surface.c"
    #include "lcairo-ud-font-face.c"
    #include "lcairo-ud-font-options.c"
    #include "lcairo-ud-ft-font-face.c"
    #include "lcairo-ud-ft-scaled-font.c"
    #include "lcairo-ud-gl-surface.c"
    #include "lcairo-ud-glitz-surface.c"
    #include "lcairo-ud-image-surface.c"
    #include "lcairo-ud-linear-gradient-pattern.c"
    #include "lcairo-ud-os2-surface.c"
    #include "lcairo-ud-path.c"
    #include "lcairo-ud-pdf-surface.c"
    #include "lcairo-ud-ps-surface.c"
    #include "lcairo-ud-qt-surface.c"
    #include "lcairo-ud-quartz-font-face.c"
    #include "lcairo-ud-quartz-image-surface.c"
    #include "lcairo-ud-quartz-surface.c"
    #include "lcairo-ud-radial-gradient-pattern.c"
    #include "lcairo-ud-region.c"
    #include "lcairo-ud-recording-surface.c"
    #include "lcairo-ud-script-surface.c"
    #include "lcairo-ud-skia-surface.c"
    #include "lcairo-ud-solid-pattern.c"
    #include "lcairo-ud-subsurface.c"
    #include "lcairo-ud-surface-pattern.c"
    #include "lcairo-ud-svg-surface.c"
    #include "lcairo-ud-tee-surface.c"
    #include "lcairo-ud-toy-font-face.c"
    #include "lcairo-ud-user-font-face.c"
    #include "lcairo-ud-vg-surface.c"
    #include "lcairo-ud-win32-font-face.c"
    #include "lcairo-ud-win32-scaled-font.c"
    #include "lcairo-ud-win32-surface.c"
    #include "lcairo-ud-xcb-surface.c"
    #include "lcairo-ud-xlib-surface.c"
    #include "lcairo-ud-xml-surface.c"
#endif /* LUACAIRO_HAS_OO_INTERFACE */

#include "lcairo-private-getters.c"


int luaopen_lcairo(lua_State* L)
{
    luaopen_lcairo_private (L);
    luaopen_lcairo_ud_font_extents (L);
    luaopen_lcairo_ud_matrix (L);
    luaopen_lcairo_ud_rectangle_int (L);
    luaopen_lcairo_ud_text_extents (L);

    luaopen_lcairo_beos_surface (L);
    luaopen_lcairo_context (L);
    luaopen_lcairo_device (L);
    luaopen_lcairo_directfb_surface (L);
    luaopen_lcairo_drm_surface (L);
    luaopen_lcairo_drm_xr (L);
    luaopen_lcairo_font_face (L);
    luaopen_lcairo_font_options (L);
    luaopen_lcairo_ft_font (L);
    luaopen_lcairo_gl_surface (L);
    luaopen_lcairo_glitz_surface (L);
    luaopen_lcairo_image_surface (L);
    luaopen_lcairo_matrix (L);
    luaopen_lcairo_os2_surface (L);
    luaopen_lcairo_path (L);
    luaopen_lcairo_pattern (L);
    luaopen_lcairo_pdf_surface (L);
    luaopen_lcairo_ps_surface (L);
    luaopen_lcairo_qt_surface (L);
    luaopen_lcairo_quartz_font_face (L);
    luaopen_lcairo_quartz_image_surface (L);
    luaopen_lcairo_quartz_surface (L);
    luaopen_lcairo_region (L);
    luaopen_lcairo_recording_surface (L);
    luaopen_lcairo_scaled_font (L);
    luaopen_lcairo_script_surface (L);
    luaopen_lcairo_skia_surface (L);
    luaopen_lcairo_surface (L);
    luaopen_lcairo_svg_surface (L);
    luaopen_lcairo_tee_surface (L);
    luaopen_lcairo_toy_font_face (L);
    luaopen_lcairo_user_font_face (L);
    luaopen_lcairo_vg_surface (L);
    luaopen_lcairo_win32_font_face (L);
    luaopen_lcairo_win32_scaled_font (L);
    luaopen_lcairo_win32_surface (L);
    luaopen_lcairo_xcb_surface (L);
    luaopen_lcairo_xlib_surface (L);
    luaopen_lcairo_xml_surface (L);

#if LUACAIRO_HAS_OO_INTERFACE
    luaopen_lcairo_ud_beos_surface (L);
    luaopen_lcairo_ud_context (L);
    luaopen_lcairo_ud_device (L);
    luaopen_lcairo_ud_directfb_surface (L);
    luaopen_lcairo_ud_drm_surface (L);
    luaopen_lcairo_ud_font_face (L);
    luaopen_lcairo_ud_font_options (L);
    luaopen_lcairo_ud_ft_font_face (L);
    luaopen_lcairo_ud_ft_scaled_font (L);
    luaopen_lcairo_ud_gl_surface (L);
    luaopen_lcairo_ud_glitz_surface (L);
    luaopen_lcairo_ud_image_surface (L);
    luaopen_lcairo_ud_linear_gradient_pattern (L);
    luaopen_lcairo_ud_os2_surface (L);
    luaopen_lcairo_ud_pattern (L);
    luaopen_lcairo_ud_path (L);
    luaopen_lcairo_ud_pdf_surface (L);
    luaopen_lcairo_ud_ps_surface (L);
    luaopen_lcairo_ud_qt_surface (L);
    luaopen_lcairo_ud_quartz_font_face (L);
    luaopen_lcairo_ud_quartz_image_surface (L);
    luaopen_lcairo_ud_quartz_surface (L);
    luaopen_lcairo_ud_radial_gradient_pattern (L);
    luaopen_lcairo_ud_recording_surface (L);
    luaopen_lcairo_ud_region (L);
    luaopen_lcairo_ud_scaled_font (L);
    luaopen_lcairo_ud_script_surface (L);
    luaopen_lcairo_ud_skia_surface (L);
    luaopen_lcairo_ud_solid_pattern (L);
    luaopen_lcairo_ud_subsurface (L);
    luaopen_lcairo_ud_surface (L);
    luaopen_lcairo_ud_surface_pattern (L);
    luaopen_lcairo_ud_svg_surface (L);
    luaopen_lcairo_ud_tee_surface (L);
    luaopen_lcairo_ud_toy_font_face (L);
    luaopen_lcairo_ud_user_font_face (L);
    luaopen_lcairo_ud_vg_surface (L);
    luaopen_lcairo_ud_win32_font_face (L);
    luaopen_lcairo_ud_win32_scaled_font (L);
    luaopen_lcairo_ud_win32_surface (L);
    luaopen_lcairo_ud_xcb_surface (L);
    luaopen_lcairo_ud_xlib_surface (L);
    luaopen_lcairo_ud_xml_surface (L);
#endif /* LUACAIRO_HAS_OO_INTERFACE */

    return 1;
}
