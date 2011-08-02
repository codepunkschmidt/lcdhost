/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif

#include "../cairo-version.h"


// --------------------------------------------------------------------------
// utilities
// --------------------------------------------------------------------------
static void * check_lightuserdata (lua_State *L, int idx);
static void * get_userdata (lua_State *L, int ud, const char *tname);
static double get_numfield (lua_State* L, const char* key);
static void   set_strfield (lua_State* L, const char* key, const char* value);
static void   set_intfield (lua_State* L, const char* key, int value);


// --------------------------------------------------------------------------
// getters
// --------------------------------------------------------------------------
static cairo_t*              get_cairo_t (lua_State *L, int idx);
static cairo_surface_t*      get_cairo_surface_t (lua_State *L, int idx);
static cairo_pattern_t*      get_cairo_pattern_t (lua_State *L, int idx);
static cairo_font_options_t* get_cairo_font_options_t (lua_State *L, int idx);
static cairo_matrix_t*       get_cairo_matrix_t (lua_State *L, int idx);
static cairo_text_extents_t* get_cairo_text_extents_t (lua_State *L, int idx);
static cairo_font_extents_t* get_cairo_font_extents_t (lua_State *L, int idx);
static cairo_scaled_font_t*  get_cairo_scaled_font_t (lua_State *L, int idx);
static cairo_font_face_t*    get_cairo_font_face_t (lua_State *L, int idx);
static cairo_path_t*         get_cairo_path_t (lua_State *L, int idx);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
static cairo_rectangle_int_t* get_cairo_rectangle_int_t (lua_State *L, int idx);
static cairo_region_t*       get_cairo_region_t (lua_State *L, int idx);
static cairo_device_t*       get_cairo_device_t (lua_State *L, int idx);
#endif


// --------------------------------------------------------------------------
// Helper functions - not part of Cairo API
// --------------------------------------------------------------------------

#if LUACAIRO_HAS_OO_INTERFACE
// oo creators for lightuserdata
// syntax: o = cairo.xxxFromLUD(lud, transfer_ownership (true/false))
static int new_ContextFromLUD (lua_State *L);
static int new_SurfaceFromLUD (lua_State *L);
static int new_PathFromLUD (lua_State *L);
static int new_PatternFromLUD (lua_State *L);
static int new_FontOptionsFromLUD (lua_State *L);
static int new_ScaledFontFromLUD (lua_State *L);
static int new_FontFaceFromLUD (lua_State *L);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
static int new_RegionFromLUD (lua_State *L);
static int new_DeviceFromLUD (lua_State *L);
#endif
#endif

//TODO should be unnecessary when all classes implemented
static void remove_Context(lua_State* L, int idx)
{
#if LUACAIRO_HAS_OO_INTERFACE
    if (get_userdata(L, idx, LUACAIRO ".Context.mt"))
    {
        lua_remove(L, idx);
    }
#endif
}


#if CAIRO_HAS_XLIB_SURFACE
    // #include <X11/Xlib.h>
    // #include <gtk/gtk.h>
    #include <gdk/gdk.h>
#endif

static int l_CreateContext(lua_State* L)
{
#if CAIRO_HAS_WIN32_SURFACE
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_t *cr = cairo_create(cairo_win32_surface_create( hdc ));
    lua_pushlightuserdata(L, cr);
    return 1;
#endif

#ifdef GDK_WINDOWING_X11
    GdkDrawable *drawable = (GdkDrawable *) check_lightuserdata(L, 1);
    cairo_t *cr = gdk_cairo_create( drawable ) ;
    lua_pushlightuserdata(L, cr);
    return 1;
#endif

    luaL_error(L, "not implemented!");
    return 0;
}

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
static int l_CreatePrintingContext(lua_State* L)
{
#if CAIRO_HAS_WIN32_SURFACE
    HDC hdc = (HDC) check_lightuserdata(L, 1);
    cairo_t *cr = cairo_create(cairo_win32_printing_surface_create( hdc ));
    lua_pushlightuserdata(L, cr);
    return 1;
#endif

#ifdef GDK_WINDOWING_X11
    luaL_error(L, "not implemented!");
#endif

    luaL_error(L, "not implemented!");
    return 0;
}
#endif




static void * check_lightuserdata(lua_State *L, int idx)
{
    luaL_checktype(L, idx, LUA_TLIGHTUSERDATA);
    return lua_touserdata(L, idx);
}

//hd.. Borrowed from lauxlib.c
//hd.. LUALIB_API void *luaL_checkudata (lua_State *L, int ud, const char *tname) {
static void* get_userdata (lua_State *L, int ud, const char *tname) {
  void *p = lua_touserdata(L, ud);
  if (p != NULL) {                                /* value is a userdata? */
    if (lua_getmetatable(L, ud)) {                /* does it have a metatable? */
      lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
      if (lua_rawequal(L, -1, -2)) {              /* does it have the correct mt? */
        lua_pop(L, 2);                            /* remove both metatables */
        return p;
      }
    }
  }
  //hd.. luaL_typerror(L, ud, tname);  /* else error */
  //hd.. return NULL;  /* to avoid warnings */
  return NULL;
}


/* PIL2:page:232,233 assume that table is at the stack top */

//unused
//static const char* get_strfield(lua_State* L, const char* key)
//{
//    lua_getfield(L, -1, key);
//    const char* value = luaL_optstring(L, -1, "");
//    lua_pop(L, 1); /* remove value */
//    return value;
//}

static double get_numfield(lua_State* L, const char* key)
{
    lua_getfield(L, -1, key);
    double value = luaL_optnumber(L, -1, 0.0);
    lua_pop(L, 1); /* remove value */
    return value;
}

//unused
//static int get_intfield(lua_State* L, const char* key)
//{
//    lua_getfield(L, -1, key);
//    int value = luaL_optinteger(L, -1, 0);
//    lua_pop(L, 1); /* remove value */
//    return value;
//}

static void set_strfield(lua_State* L, const char* key, const char* value)
{
    lua_pushstring(L, value);
    lua_setfield(L, -2, key);
}

//unused
//static void set_numfield(lua_State* L, const char* key, double value)
//{
//    lua_pushnumber(L, value);
//    lua_setfield(L, -2, key);
//}

static void set_intfield(lua_State* L, const char* key, int value)
{
    lua_pushinteger(L, value);
    lua_setfield(L, -2, key);
}




// --------------------------------------------------------------------------
// Utilities for struct binding
// Used for cairo_matrix_t, cairo_text_extents_t, cairo_font_extents_t
// Adapted from: http://lua-users.org/wiki/BindingWithMembersAndMethods
// --------------------------------------------------------------------------

//unused
//static int Xet_get_int (lua_State *L, void *v)
//{
//    lua_pushnumber(L, *(int*)v);
//    return 1;
//}

//unused
//static int Xet_set_int (lua_State *L, void *v)
//{
//    *(int*)v = luaL_checkint(L, 3);
//    return 0;
//}

static int Xet_get_number (lua_State *L, void *v)
{
    lua_pushnumber(L, *(lua_Number*)v);
    return 1;
}

static int Xet_set_number (lua_State *L, void *v)
{
    *(lua_Number*)v = luaL_checknumber(L, 3);
    return 0;
}

//unused
//static int Xet_get_string (lua_State *L, void *v)
//{
//    lua_pushstring(L, (char*)v );
//    return 1;
//}

typedef int (*Xet_func) (lua_State *L, void *v);

/* member info for get and set handlers */
typedef const struct _Xet_reg_pre {
    const char *name;       /* member name */
    Xet_func    func;       /* get or set function for type of member */
    size_t      offset;     /* offset of member within your_t */
}  Xet_reg_pre;

typedef Xet_reg_pre * Xet_reg;

static void Xet_add (lua_State *L, Xet_reg l)
{
    for (; l->name; l++) {
        lua_pushstring(L, l->name);
        lua_pushlightuserdata(L, (void*)l);
        lua_settable(L, -3);
    }
}

static int Xet_call (lua_State *L)
{
    /* for get: stack has userdata, index, lightuserdata */
    /* for set: stack has userdata, index, value, lightuserdata */
    Xet_reg m = (Xet_reg)lua_touserdata(L, -1);  /* member info */
    lua_pop(L, 1);                               /* drop lightuserdata */
    luaL_checktype(L, 1, LUA_TUSERDATA);
    return m->func(L, (void *)((char *)lua_touserdata(L, 1) + m->offset));
}

static int Xet_index_handler (lua_State *L)
{
    /* stack has userdata, index */
    lua_pushvalue(L, 2);                       /* dup index */
    lua_rawget(L, lua_upvalueindex(1));        /* lookup member by name */
    if (!lua_islightuserdata(L, -1)) {
        lua_pop(L, 1);                         /* drop value */
        lua_pushvalue(L, 2);                   /* dup index */
        lua_gettable(L, lua_upvalueindex(2));  /* else try methods */
        if (lua_isnil(L, -1))                  /* invalid member */
            luaL_error(L, "cannot get member '%s'", lua_tostring(L, 2));
        return 1;
    }
    return Xet_call(L);                        /* call get function */
}

static int Xet_newindex_handler (lua_State *L)
{
    /* stack has userdata, index, value */
    lua_pushvalue(L, 2);                       /* dup index */
    lua_rawget(L, lua_upvalueindex(1));        /* lookup member by name */
    if (!lua_islightuserdata(L, -1))           /* invalid member */
        luaL_error(L, "cannot set member '%s'", lua_tostring(L, 2));
    return Xet_call(L);                        /* call set function */
}




static const struct luaL_Reg private_f [] = {
    {"CreateContext",                            l_CreateContext},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    {"CreatePrintingContext",                    l_CreatePrintingContext},
#endif

#if LUACAIRO_HAS_OO_INTERFACE
    {"ContextFromLUD",                           new_ContextFromLUD},
    {"SurfaceFromLUD",                           new_SurfaceFromLUD},
    {"PathFromLUD",                              new_PathFromLUD},
    {"PatternFromLUD",                           new_PatternFromLUD},
    {"FontOptionsFromLUD",                       new_FontOptionsFromLUD},
    {"ScaledFontFromLUD",                        new_ScaledFontFromLUD},
    {"FontFaceFromLUD",                          new_FontFaceFromLUD},
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    {"RegionFromLUD",                            new_RegionFromLUD},
    {"DeviceFromLUD",                            new_DeviceFromLUD},
#endif
#endif

	{NULL, NULL} /* sentinel */
};



static int luaopen_lcairo_private(lua_State* L)
{
    // register functions
    luaL_register(L, LUACAIRO, private_f);


    // register constants
    //
    // trick by: Jerome Vuarand (jerome.vuarand@ubisoft.com)
    // local cairo = require 'lcario'
    // local CAIRO = cairo
    // cairo.set_operator (cr, CAIRO.OPERATOR_ADD)
    //

    set_strfield(L, "VERSION_STRING",                CAIRO_VERSION_STRING);
    set_intfield(L, "VERSION_MAJOR",                 CAIRO_VERSION_MAJOR);
    set_intfield(L, "VERSION_MINOR",                 CAIRO_VERSION_MINOR);
    set_intfield(L, "VERSION_MICRO",                 CAIRO_VERSION_MICRO);
    set_intfield(L, "VERSION",                       CAIRO_VERSION);

    set_intfield(L, "HAS_XCB_DRM_FUNCTIONS",         CAIRO_HAS_XCB_DRM_FUNCTIONS);
    set_intfield(L, "HAS_XCB_SHM_FUNCTIONS",         CAIRO_HAS_XCB_SHM_FUNCTIONS);
    set_intfield(L, "HAS_XLIB_XCB_FUNCTIONS",        CAIRO_HAS_XLIB_XCB_FUNCTIONS);
    set_intfield(L, "HAS_RECORDING_SURFACE",         CAIRO_HAS_RECORDING_SURFACE);
    set_intfield(L, "HAS_GALLIUM_SURFACE",           CAIRO_HAS_GALLIUM_SURFACE);
    set_intfield(L, "HAS_SYMBOL_LOOKUP",             CAIRO_HAS_SYMBOL_LOOKUP);
    set_intfield(L, "HAS_PTHREAD",                   CAIRO_HAS_PTHREAD);
    set_intfield(L, "HAS_GOBJECT_FUNCTIONS",         CAIRO_HAS_GOBJECT_FUNCTIONS);
    set_intfield(L, "HAS_TRACE",                     CAIRO_HAS_TRACE);
    set_intfield(L, "HAS_INTERPRETER",               CAIRO_HAS_INTERPRETER);
    set_intfield(L, "HAS_BEOS_SURFACE",              CAIRO_HAS_BEOS_SURFACE);
    set_intfield(L, "HAS_DIRECTFB_SURFACE",          CAIRO_HAS_DIRECTFB_SURFACE);
    set_intfield(L, "HAS_DRM_SURFACE",               CAIRO_HAS_DRM_SURFACE);
    set_intfield(L, "HAS_DRM_XR_FUNCTIONS",          CAIRO_HAS_DRM_XR_FUNCTIONS);
    set_intfield(L, "HAS_EGL_FUNCTIONS",             CAIRO_HAS_EGL_FUNCTIONS);
    set_intfield(L, "HAS_FT_FONT",                   CAIRO_HAS_FT_FONT);
    set_intfield(L, "HAS_FC_FONT",                   CAIRO_HAS_FC_FONT);
    set_intfield(L, "HAS_GL_SURFACE",                CAIRO_HAS_GL_SURFACE);
    set_intfield(L, "HAS_GLITZ_SURFACE",             CAIRO_HAS_GLITZ_SURFACE);
    set_intfield(L, "HAS_GLX_FUNCTIONS",             CAIRO_HAS_GLX_FUNCTIONS);
    set_intfield(L, "HAS_OS2_SURFACE",               CAIRO_HAS_OS2_SURFACE);
    set_intfield(L, "HAS_PDF_SURFACE",               CAIRO_HAS_PDF_SURFACE);
    set_intfield(L, "HAS_PNG_FUNCTIONS",             CAIRO_HAS_PNG_FUNCTIONS);
    set_intfield(L, "HAS_PS_SURFACE",                CAIRO_HAS_PS_SURFACE);
    set_intfield(L, "HAS_QT_SURFACE",                CAIRO_HAS_QT_SURFACE);
    set_intfield(L, "HAS_QUARTZ_FONT",               CAIRO_HAS_QUARTZ_FONT);
    set_intfield(L, "HAS_QUARTZ_IMAGE_SURFACE",      CAIRO_HAS_QUARTZ_IMAGE_SURFACE);
    set_intfield(L, "HAS_QUARTZ_SURFACE",            CAIRO_HAS_QUARTZ_SURFACE);
    set_intfield(L, "HAS_SCRIPT_SURFACE",            CAIRO_HAS_SCRIPT_SURFACE);
    set_intfield(L, "HAS_SKIA_SURFACE",              CAIRO_HAS_SKIA_SURFACE);
    set_intfield(L, "HAS_SVG_SURFACE",               CAIRO_HAS_SVG_SURFACE);
    set_intfield(L, "HAS_TEE_SURFACE",               CAIRO_HAS_TEE_SURFACE);
    set_intfield(L, "HAS_TEST_SURFACES",             CAIRO_HAS_TEST_SURFACES);
    set_intfield(L, "HAS_USER_FONT",                 CAIRO_HAS_USER_FONT);
    set_intfield(L, "HAS_VG_SURFACE",                CAIRO_HAS_VG_SURFACE);
    set_intfield(L, "HAS_WGL_FUNCTIONS",             CAIRO_HAS_WGL_FUNCTIONS);
    set_intfield(L, "HAS_WIN32_SURFACE",             CAIRO_HAS_WIN32_SURFACE);
    set_intfield(L, "HAS_WIN32_FONT",                CAIRO_HAS_WIN32_FONT);
    set_intfield(L, "HAS_XCB_SURFACE",               CAIRO_HAS_XCB_SURFACE);
    set_intfield(L, "HAS_XLIB_SURFACE",              CAIRO_HAS_XLIB_SURFACE);
    set_intfield(L, "HAS_XLIB_XRENDER_SURFACE",      CAIRO_HAS_XLIB_XRENDER_SURFACE);
    set_intfield(L, "HAS_XML_SURFACE",               CAIRO_HAS_XML_SURFACE);

    set_intfield(L, "STATUS_SUCCESS",                CAIRO_STATUS_SUCCESS);
    set_intfield(L, "STATUS_NO_MEMORY",              CAIRO_STATUS_NO_MEMORY);
    set_intfield(L, "STATUS_INVALID_RESTORE",        CAIRO_STATUS_INVALID_RESTORE);
    set_intfield(L, "STATUS_INVALID_POP_GROUP",      CAIRO_STATUS_INVALID_POP_GROUP);
    set_intfield(L, "STATUS_NO_CURRENT_POINT",       CAIRO_STATUS_NO_CURRENT_POINT);
    set_intfield(L, "STATUS_INVALID_MATRIX",         CAIRO_STATUS_INVALID_MATRIX);
    set_intfield(L, "STATUS_INVALID_STATUS",         CAIRO_STATUS_INVALID_STATUS);
    set_intfield(L, "STATUS_NULL_POINTER",           CAIRO_STATUS_NULL_POINTER);
    set_intfield(L, "STATUS_INVALID_STRING",         CAIRO_STATUS_INVALID_STRING);
    set_intfield(L, "STATUS_INVALID_PATH_DATA",      CAIRO_STATUS_INVALID_PATH_DATA);
    set_intfield(L, "STATUS_READ_ERROR",             CAIRO_STATUS_READ_ERROR);
    set_intfield(L, "STATUS_WRITE_ERROR",            CAIRO_STATUS_WRITE_ERROR);
    set_intfield(L, "STATUS_SURFACE_FINISHED",       CAIRO_STATUS_SURFACE_FINISHED);
    set_intfield(L, "STATUS_SURFACE_TYPE_MISMATCH",  CAIRO_STATUS_SURFACE_TYPE_MISMATCH);
    set_intfield(L, "STATUS_PATTERN_TYPE_MISMATCH",  CAIRO_STATUS_PATTERN_TYPE_MISMATCH);
    set_intfield(L, "STATUS_INVALID_CONTENT",        CAIRO_STATUS_INVALID_CONTENT);
    set_intfield(L, "STATUS_INVALID_FORMAT",         CAIRO_STATUS_INVALID_FORMAT);
    set_intfield(L, "STATUS_INVALID_VISUAL",         CAIRO_STATUS_INVALID_VISUAL);
    set_intfield(L, "STATUS_FILE_NOT_FOUND",         CAIRO_STATUS_FILE_NOT_FOUND);
    set_intfield(L, "STATUS_INVALID_DASH",           CAIRO_STATUS_INVALID_DASH);
    set_intfield(L, "STATUS_INVALID_DSC_COMMENT",    CAIRO_STATUS_INVALID_DSC_COMMENT);
    set_intfield(L, "STATUS_INVALID_INDEX",          CAIRO_STATUS_INVALID_INDEX);
    set_intfield(L, "STATUS_CLIP_NOT_REPRESENTABLE", CAIRO_STATUS_CLIP_NOT_REPRESENTABLE);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    set_intfield(L, "STATUS_TEMP_FILE_ERROR",        CAIRO_STATUS_TEMP_FILE_ERROR);
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    set_intfield(L, "STATUS_INVALID_STRIDE",         CAIRO_STATUS_INVALID_STRIDE);
    set_intfield(L, "STATUS_FONT_TYPE_MISMATCH",     CAIRO_STATUS_FONT_TYPE_MISMATCH);
    set_intfield(L, "STATUS_USER_FONT_IMMUTABLE",    CAIRO_STATUS_USER_FONT_IMMUTABLE);
    set_intfield(L, "STATUS_USER_FONT_ERROR",        CAIRO_STATUS_USER_FONT_ERROR);
    set_intfield(L, "STATUS_NEGATIVE_COUNT",         CAIRO_STATUS_NEGATIVE_COUNT);
    set_intfield(L, "STATUS_INVALID_CLUSTERS",       CAIRO_STATUS_INVALID_CLUSTERS);
    set_intfield(L, "STATUS_INVALID_SLANT",          CAIRO_STATUS_INVALID_SLANT);
    set_intfield(L, "STATUS_INVALID_WEIGHT",         CAIRO_STATUS_INVALID_WEIGHT);
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "STATUS_INVALID_SIZE",           CAIRO_STATUS_INVALID_SIZE);
    set_intfield(L, "STATUS_USER_FONT_NOT_IMPLEMENTED", CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED);
    set_intfield(L, "STATUS_DEVICE_TYPE_MISMATCH",   CAIRO_STATUS_DEVICE_TYPE_MISMATCH);
    set_intfield(L, "STATUS_DEVICE_ERROR",           CAIRO_STATUS_DEVICE_ERROR);
#endif

    set_intfield(L, "CONTENT_COLOR",                 CAIRO_CONTENT_COLOR);
    set_intfield(L, "CONTENT_ALPHA",                 CAIRO_CONTENT_ALPHA);
    set_intfield(L, "CONTENT_COLOR_ALPHA",           CAIRO_CONTENT_COLOR_ALPHA);

    set_intfield(L, "OPERATOR_CLEAR",                CAIRO_OPERATOR_CLEAR);
    set_intfield(L, "OPERATOR_SOURCE",               CAIRO_OPERATOR_SOURCE);
    set_intfield(L, "OPERATOR_OVER",                 CAIRO_OPERATOR_OVER);
    set_intfield(L, "OPERATOR_IN",                   CAIRO_OPERATOR_IN);
    set_intfield(L, "OPERATOR_OUT",                  CAIRO_OPERATOR_OUT);
    set_intfield(L, "OPERATOR_ATOP",                 CAIRO_OPERATOR_ATOP);
    set_intfield(L, "OPERATOR_DEST",                 CAIRO_OPERATOR_DEST);
    set_intfield(L, "OPERATOR_DEST_OVER",            CAIRO_OPERATOR_DEST_OVER);
    set_intfield(L, "OPERATOR_DEST_IN",              CAIRO_OPERATOR_DEST_IN);
    set_intfield(L, "OPERATOR_DEST_OUT",             CAIRO_OPERATOR_DEST_OUT);
    set_intfield(L, "OPERATOR_DEST_ATOP",            CAIRO_OPERATOR_DEST_ATOP);
    set_intfield(L, "OPERATOR_XOR",                  CAIRO_OPERATOR_XOR);
    set_intfield(L, "OPERATOR_ADD",                  CAIRO_OPERATOR_ADD);
    set_intfield(L, "OPERATOR_SATURATE",             CAIRO_OPERATOR_SATURATE);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "OPERATOR_MULTIPLY",             CAIRO_OPERATOR_MULTIPLY);
    set_intfield(L, "OPERATOR_SCREEN",               CAIRO_OPERATOR_SCREEN);
    set_intfield(L, "OPERATOR_OVERLAY",              CAIRO_OPERATOR_OVERLAY);
    set_intfield(L, "OPERATOR_DARKEN",               CAIRO_OPERATOR_DARKEN);
    set_intfield(L, "OPERATOR_LIGHTEN",              CAIRO_OPERATOR_LIGHTEN);
    set_intfield(L, "OPERATOR_COLOR_DODGE",          CAIRO_OPERATOR_COLOR_DODGE);
    set_intfield(L, "OPERATOR_COLOR_BURN",           CAIRO_OPERATOR_COLOR_BURN);
    set_intfield(L, "OPERATOR_HARD_LIGHT",           CAIRO_OPERATOR_HARD_LIGHT);
    set_intfield(L, "OPERATOR_SOFT_LIGHT",           CAIRO_OPERATOR_SOFT_LIGHT);
    set_intfield(L, "OPERATOR_DIFFERENCE",           CAIRO_OPERATOR_DIFFERENCE);
    set_intfield(L, "OPERATOR_EXCLUSION",            CAIRO_OPERATOR_EXCLUSION);
    set_intfield(L, "OPERATOR_HSL_HUE",              CAIRO_OPERATOR_HSL_HUE);
    set_intfield(L, "OPERATOR_HSL_SATURATION",       CAIRO_OPERATOR_HSL_SATURATION);
    set_intfield(L, "OPERATOR_HSL_COLOR",            CAIRO_OPERATOR_HSL_COLOR);
    set_intfield(L, "OPERATOR_HSL_LUMINOSITY",       CAIRO_OPERATOR_HSL_LUMINOSITY);
#endif

    set_intfield(L, "ANTIALIAS_DEFAULT",             CAIRO_ANTIALIAS_DEFAULT);
    set_intfield(L, "ANTIALIAS_NONE",                CAIRO_ANTIALIAS_NONE);
    set_intfield(L, "ANTIALIAS_GRAY",                CAIRO_ANTIALIAS_GRAY);
    set_intfield(L, "ANTIALIAS_SUBPIXEL",            CAIRO_ANTIALIAS_SUBPIXEL);

    set_intfield(L, "FILL_RULE_WINDING",             CAIRO_FILL_RULE_WINDING);
    set_intfield(L, "FILL_RULE_EVEN_ODD",            CAIRO_FILL_RULE_EVEN_ODD);

    set_intfield(L, "LINE_CAP_BUTT",                 CAIRO_LINE_CAP_BUTT);
    set_intfield(L, "LINE_CAP_ROUND",                CAIRO_LINE_CAP_ROUND);
    set_intfield(L, "LINE_CAP_SQUARE",               CAIRO_LINE_CAP_SQUARE);

    set_intfield(L, "LINE_JOIN_MITER",               CAIRO_LINE_JOIN_MITER);
    set_intfield(L, "LINE_JOIN_ROUND",               CAIRO_LINE_JOIN_ROUND);
    set_intfield(L, "LINE_JOIN_BEVEL",               CAIRO_LINE_JOIN_BEVEL);

    set_intfield(L, "FONT_SLANT_NORMAL",             CAIRO_FONT_SLANT_NORMAL);
    set_intfield(L, "FONT_SLANT_ITALIC",             CAIRO_FONT_SLANT_ITALIC);
    set_intfield(L, "FONT_SLANT_OBLIQUE",            CAIRO_FONT_SLANT_OBLIQUE);

    set_intfield(L, "FONT_WEIGHT_NORMAL",            CAIRO_FONT_WEIGHT_NORMAL);
    set_intfield(L, "FONT_WEIGHT_BOLD",              CAIRO_FONT_WEIGHT_BOLD);

    set_intfield(L, "SUBPIXEL_ORDER_DEFAULT",        CAIRO_SUBPIXEL_ORDER_DEFAULT);
    set_intfield(L, "SUBPIXEL_ORDER_RGB",            CAIRO_SUBPIXEL_ORDER_RGB);
    set_intfield(L, "SUBPIXEL_ORDER_BGR",            CAIRO_SUBPIXEL_ORDER_BGR);
    set_intfield(L, "SUBPIXEL_ORDER_VRGB",           CAIRO_SUBPIXEL_ORDER_VRGB);
    set_intfield(L, "SUBPIXEL_ORDER_VBGR",           CAIRO_SUBPIXEL_ORDER_VBGR);

    set_intfield(L, "HINT_STYLE_DEFAULT",            CAIRO_HINT_STYLE_DEFAULT);
    set_intfield(L, "HINT_STYLE_NONE",               CAIRO_HINT_STYLE_NONE);
    set_intfield(L, "HINT_STYLE_SLIGHT",             CAIRO_HINT_STYLE_SLIGHT);
    set_intfield(L, "HINT_STYLE_MEDIUM",             CAIRO_HINT_STYLE_MEDIUM);
    set_intfield(L, "HINT_STYLE_FULL",               CAIRO_HINT_STYLE_FULL);

    set_intfield(L, "HINT_METRICS_DEFAULT",          CAIRO_HINT_METRICS_DEFAULT);
    set_intfield(L, "HINT_METRICS_OFF",              CAIRO_HINT_METRICS_OFF);
    set_intfield(L, "HINT_METRICS_ON",               CAIRO_HINT_METRICS_ON);

    set_intfield(L, "FONT_TYPE_TOY",                 CAIRO_FONT_TYPE_TOY);
    set_intfield(L, "FONT_TYPE_FT",                  CAIRO_FONT_TYPE_FT);
    set_intfield(L, "FONT_TYPE_WIN32",               CAIRO_FONT_TYPE_WIN32);
#if CAIRO_VERSION > CAIRO_VERSION_ENCODE(1, 6, 0)
    set_intfield(L, "FONT_TYPE_QUARTZ",              CAIRO_FONT_TYPE_QUARTZ);
#endif
#if CAIRO_VERSION > CAIRO_VERSION_ENCODE(1, 8, 0)
    set_intfield(L, "FONT_TYPE_USER",                CAIRO_FONT_TYPE_USER);
#endif

    set_intfield(L, "PATH_MOVE_TO",                  CAIRO_PATH_MOVE_TO);
    set_intfield(L, "PATH_LINE_TO",                  CAIRO_PATH_LINE_TO);
    set_intfield(L, "PATH_CURVE_TO",                 CAIRO_PATH_CURVE_TO);
    set_intfield(L, "PATH_CLOSE_PATH",               CAIRO_PATH_CLOSE_PATH);

    set_intfield(L, "SURFACE_TYPE_IMAGE",            CAIRO_SURFACE_TYPE_IMAGE);
    set_intfield(L, "SURFACE_TYPE_PDF",              CAIRO_SURFACE_TYPE_PDF);
    set_intfield(L, "SURFACE_TYPE_PS",               CAIRO_SURFACE_TYPE_PS);
    set_intfield(L, "SURFACE_TYPE_XLIB",             CAIRO_SURFACE_TYPE_XLIB);
    set_intfield(L, "SURFACE_TYPE_XCB",              CAIRO_SURFACE_TYPE_XCB);
    set_intfield(L, "SURFACE_TYPE_GLITZ",            CAIRO_SURFACE_TYPE_GLITZ);
    set_intfield(L, "SURFACE_TYPE_QUARTZ",           CAIRO_SURFACE_TYPE_QUARTZ);
    set_intfield(L, "SURFACE_TYPE_WIN32",            CAIRO_SURFACE_TYPE_WIN32);
    set_intfield(L, "SURFACE_TYPE_BEOS",             CAIRO_SURFACE_TYPE_BEOS);
    set_intfield(L, "SURFACE_TYPE_DIRECTFB",         CAIRO_SURFACE_TYPE_DIRECTFB);
    set_intfield(L, "SURFACE_TYPE_SVG",              CAIRO_SURFACE_TYPE_SVG);
    set_intfield(L, "SURFACE_TYPE_OS2",              CAIRO_SURFACE_TYPE_OS2);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    set_intfield(L, "SURFACE_TYPE_WIN32_PRINTING",   CAIRO_SURFACE_TYPE_WIN32_PRINTING);
    set_intfield(L, "SURFACE_TYPE_QUARTZ_IMAGE",     CAIRO_SURFACE_TYPE_QUARTZ_IMAGE);
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "SURFACE_TYPE_SCRIPT",           CAIRO_SURFACE_TYPE_SCRIPT);
    set_intfield(L, "SURFACE_TYPE_QT",               CAIRO_SURFACE_TYPE_QT);
    set_intfield(L, "SURFACE_TYPE_RECORDING",        CAIRO_SURFACE_TYPE_RECORDING);
    set_intfield(L, "SURFACE_TYPE_VG",               CAIRO_SURFACE_TYPE_VG);
    set_intfield(L, "SURFACE_TYPE_GL",               CAIRO_SURFACE_TYPE_GL);
    set_intfield(L, "SURFACE_TYPE_DRM",              CAIRO_SURFACE_TYPE_DRM);
    set_intfield(L, "SURFACE_TYPE_TEE",              CAIRO_SURFACE_TYPE_TEE);
    set_intfield(L, "SURFACE_TYPE_XML",              CAIRO_SURFACE_TYPE_XML);
    set_intfield(L, "SURFACE_TYPE_SKIA",             CAIRO_SURFACE_TYPE_SKIA);
    set_intfield(L, "SURFACE_TYPE_SUBSURFACE",       CAIRO_SURFACE_TYPE_SUBSURFACE);
#endif

    set_intfield(L, "FORMAT_ARGB32",                 CAIRO_FORMAT_ARGB32);
    set_intfield(L, "FORMAT_RGB24",                  CAIRO_FORMAT_RGB24);
    set_intfield(L, "FORMAT_A8",                     CAIRO_FORMAT_A8);
    set_intfield(L, "FORMAT_A1",                     CAIRO_FORMAT_A1);

    set_intfield(L, "PATTERN_TYPE_SOLID",            CAIRO_PATTERN_TYPE_SOLID);
    set_intfield(L, "PATTERN_TYPE_SURFACE",          CAIRO_PATTERN_TYPE_SURFACE);
    set_intfield(L, "PATTERN_TYPE_LINEAR",           CAIRO_PATTERN_TYPE_LINEAR);
    set_intfield(L, "PATTERN_TYPE_RADIAL",           CAIRO_PATTERN_TYPE_RADIAL);

    set_intfield(L, "EXTEND_NONE",                   CAIRO_EXTEND_NONE);
    set_intfield(L, "EXTEND_REPEAT",                 CAIRO_EXTEND_REPEAT);
    set_intfield(L, "EXTEND_REFLECT",                CAIRO_EXTEND_REFLECT);
    set_intfield(L, "EXTEND_PAD",                    CAIRO_EXTEND_PAD);

    set_intfield(L, "FILTER_FAST",                   CAIRO_FILTER_FAST);
    set_intfield(L, "FILTER_GOOD",                   CAIRO_FILTER_GOOD);
    set_intfield(L, "FILTER_BEST",                   CAIRO_FILTER_BEST);
    set_intfield(L, "FILTER_NEAREST",                CAIRO_FILTER_NEAREST);
    set_intfield(L, "FILTER_BILINEAR",               CAIRO_FILTER_BILINEAR);
    set_intfield(L, "FILTER_GAUSSIAN",               CAIRO_FILTER_GAUSSIAN);

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_strfield(L, "MIME_TYPE_JPEG",                CAIRO_MIME_TYPE_JPEG);
    set_strfield(L, "MIME_TYPE_PNG",                 CAIRO_MIME_TYPE_PNG);
    set_strfield(L, "MIME_TYPE_JP2",                 CAIRO_MIME_TYPE_JP2);
    set_strfield(L, "MIME_TYPE_URI",                 CAIRO_MIME_TYPE_URI);
#endif

#if CAIRO_HAS_SCRIPT_SURFACE
    set_intfield(L, "SCRIPT_MODE_BINARY",            CAIRO_SCRIPT_MODE_BINARY);
    set_intfield(L, "SCRIPT_MODE_ASCII",             CAIRO_SCRIPT_MODE_ASCII);
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "FORMAT_INVALID",                CAIRO_FORMAT_INVALID);
    set_intfield(L, "FORMAT_ARGB32",                 CAIRO_FORMAT_ARGB32);
    set_intfield(L, "FORMAT_RGB24",                  CAIRO_FORMAT_RGB24);
    set_intfield(L, "FORMAT_A8",                     CAIRO_FORMAT_A8);
    set_intfield(L, "FORMAT_A1",                     CAIRO_FORMAT_A1);
    set_intfield(L, "FORMAT_RGB16_565",              CAIRO_FORMAT_RGB16_565);
#endif

#if CAIRO_HAS_SVG_SURFACE
    set_intfield(L, "SVG_VERSION_1_1",               CAIRO_SVG_VERSION_1_1);
    set_intfield(L, "SVG_VERSION_1_2",               CAIRO_SVG_VERSION_1_2);
#endif

#if CAIRO_HAS_PS_SURFACE
    set_intfield(L, "PS_LEVEL_2",                    CAIRO_PS_LEVEL_2);
    set_intfield(L, "PS_LEVEL_3",                    CAIRO_PS_LEVEL_3);
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0)
    set_intfield(L, "TEXT_CLUSTER_FLAG_BACKWARD",    CAIRO_TEXT_CLUSTER_FLAG_BACKWARD);
#endif

#if CAIRO_HAS_PDF_SURFACE
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "PDF_VERSION_1_4",               CAIRO_PDF_VERSION_1_4);
    set_intfield(L, "PDF_VERSION_1_5",               CAIRO_PDF_VERSION_1_5);
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 8, 0) */
#endif

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "DEVICE_TYPE_DRM",               CAIRO_DEVICE_TYPE_DRM);
    set_intfield(L, "DEVICE_TYPE_GL",                CAIRO_DEVICE_TYPE_GL);
    set_intfield(L, "DEVICE_TYPE_SCRIPT",            CAIRO_DEVICE_TYPE_SCRIPT);
    set_intfield(L, "DEVICE_TYPE_XCB",               CAIRO_DEVICE_TYPE_XCB);
    set_intfield(L, "DEVICE_TYPE_XLIB",              CAIRO_DEVICE_TYPE_XLIB);
    set_intfield(L, "DEVICE_TYPE_XML",               CAIRO_DEVICE_TYPE_XML);
#endif /* CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0) */

#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    set_intfield(L, "REGION_OVERLAP_IN",             CAIRO_REGION_OVERLAP_IN);
    set_intfield(L, "REGION_OVERLAP_OUT",            CAIRO_REGION_OVERLAP_OUT);
    set_intfield(L, "REGION_OVERLAP_PART",           CAIRO_REGION_OVERLAP_PART);		
#endif

    return 0;
}
