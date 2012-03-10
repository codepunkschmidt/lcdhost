/* -*- C -*-
 *
 * Lua binding of Cairo
 *
 */
#ifndef LUACAIRO
# error private include file
#endif



// --------------------------------------------------------------------------
// Surface userdata for cairo_surface_t
// --------------------------------------------------------------------------



#if LUACAIRO_HAS_OO_INTERFACE

typedef struct _Surface
{
    cairo_surface_t*  cs_;
    int               havecs_;
    int               typecs_;
} Surface;


static Surface* check_Surface_ud(lua_State *L, int idx)
{
    Surface* cs = NULL;

#if CAIRO_HAS_IMAGE_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".ImageSurface.mt");
    // if (cs == NULL) printf("cs is not ImageSurface\n");
#endif

#if CAIRO_HAS_PDF_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".PdfSurface.mt");
    // if (cs == NULL) printf("cs is not PdfSurface\n");
#endif

#if CAIRO_HAS_PS_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".PsSurface.mt");
    // if (cs == NULL) printf("cs is not PsSurface\n");
#endif

#if CAIRO_HAS_XLIB_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".XlibSurface.mt");
    // if (cs == NULL) printf("cs is not XlibSurface\n");
#endif

#if CAIRO_HAS_XCB_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".XcbSurface.mt");
    // if (cs == NULL) printf("cs is not XcbSurface\n");
#endif

#if CAIRO_HAS_GLITZ_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".GlitzSurface.mt");
    // if (cs == NULL) printf("cs is not GlitzSurface\n");
#endif

#if CAIRO_HAS_QUARTZ_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".QuartzSurface.mt");
    // if (cs == NULL) printf("cs is not QuartzSurface\n");
#endif

#if CAIRO_HAS_WIN32_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".Win32Surface.mt");
    // if (cs == NULL) printf("cs is not Win32Surface\n");
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".Win32PrintingSurface.mt");
    // if (cs == NULL) printf("cs is not Win32PrintingSurface\n");
#endif
#endif

#if CAIRO_HAS_BEOS_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".BeosSurface.mt");
    // if (cs == NULL) printf("cs is not BeosSurface\n");
#endif

#if CAIRO_HAS_DIRECTFB_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".DirectFbSurface.mt");
    // if (cs == NULL) printf("cs is not DirectFbSurface\n");
#endif

#if CAIRO_HAS_SVG_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".SvgSurface.mt");
    // if (cs == NULL) printf("cs is not SvgSurface\n");
#endif

#if CAIRO_HAS_OS2_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".Os2Surface.mt");
    // if (cs == NULL) printf("cs is not Os2Surface\n");
#endif

#if CAIRO_HAS_QUARTZ_IMAGE_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".QuartzImageSurface.mt");
    // if (cs == NULL) printf("cs is not QuartzImageSurface\n");
#endif

#if CAIRO_HAS_RECORDING_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".RecordingSurface.mt");
    // if (cs == NULL) printf("cs is not RecordingSurface\n");
#endif

#if CAIRO_HAS_GALLIUM_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".GalliumSurface.mt");
    // if (cs == NULL) printf("cs is not GalliumSurface\n");
#endif

#if CAIRO_HAS_DRM_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".DrmSurface.mt");
    // if (cs == NULL) printf("cs is not DrmSurface\n");
#endif

#if CAIRO_HAS_GL_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".GlSurface.mt");
    // if (cs == NULL) printf("cs is not GlSurface\n");
#endif

#if CAIRO_HAS_QT_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".QtSurface.mt");
    // if (cs == NULL) printf("cs is not QtSurface\n");
#endif

#if CAIRO_HAS_SCRIPT_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".ScriptSurface.mt");
    // if (cs == NULL) printf("cs is not ScriptSurface\n");
#endif

#if CAIRO_HAS_SKIA_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".SkiaSurface.mt");
    // if (cs == NULL) printf("cs is not SkiaSurface\n");
#endif

#if CAIRO_HAS_TEE_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".TeeSurface.mt");
    // if (cs == NULL) printf("cs is not TeeSurface\n");
#endif

#if CAIRO_HAS_VG_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".VgSurface.mt");
    // if (cs == NULL) printf("cs is not VgSurface\n");
#endif

#if CAIRO_HAS_XLIB_XRENDER_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".XlibXrenderSurface.mt");
    // if (cs == NULL) printf("cs is not XlibXrenderSurface\n");
#endif

#if CAIRO_HAS_XML_SURFACE
    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".XmlSurface.mt");
    // if (cs == NULL) printf("cs is not XmlSurface\n");
#endif

    if (cs == NULL) cs = (Surface*) get_userdata(L, idx, LUACAIRO ".SubSurface.mt");
    // if (cs == NULL) printf("cs is not SubSurface\n");

    if (cs == NULL) luaL_typerror(L, idx, LUACAIRO ".Surface");

    return cs;
}

static int new_Surface (lua_State *L, const char *tname, cairo_surface_t* cs, int typecs, int havecs)
{
    Surface *o = (Surface *) lua_newuserdata(L, sizeof(Surface));
    o->cs_     = cs;
    o->havecs_ = havecs;
    o->typecs_ = typecs;

    luaL_getmetatable(L, tname);
    lua_setmetatable(L, -2);

    return 1;
}

static int new_SurfaceFromLUD (lua_State *L)
{
    cairo_surface_t *cs = (cairo_surface_t *) check_lightuserdata(L, 1);
    int havecs          = lua_toboolean(L, 2);
    int typecs          = cairo_surface_get_type(cs);

    switch (typecs) {
    case CAIRO_SURFACE_TYPE_IMAGE:          return new_Surface(L, LUACAIRO ".ImageSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_PDF:            return new_Surface(L, LUACAIRO ".PdfSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_PS:             return new_Surface(L, LUACAIRO ".PsSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_XLIB:           return new_Surface(L, LUACAIRO ".XlibSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_XCB:            return new_Surface(L, LUACAIRO ".XcbSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_GLITZ:          return new_Surface(L, LUACAIRO ".GlitzSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_QUARTZ:         return new_Surface(L, LUACAIRO ".QuartzSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_WIN32:          return new_Surface(L, LUACAIRO ".Win32Surface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_BEOS:           return new_Surface(L, LUACAIRO ".BeosSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_DIRECTFB:       return new_Surface(L, LUACAIRO ".DirectFbSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_SVG:            return new_Surface(L, LUACAIRO ".SvgSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_OS2:            return new_Surface(L, LUACAIRO ".Os2Surface.mt", cs, typecs, havecs);
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 6, 0)
    case CAIRO_SURFACE_TYPE_WIN32_PRINTING: return new_Surface(L, LUACAIRO ".Win32PrintingSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_QUARTZ_IMAGE:   return new_Surface(L, LUACAIRO ".QuartzImageSurface.mt", cs, typecs, havecs);
#endif
#if CAIRO_VERSION >= CAIRO_VERSION_ENCODE(1, 10, 0)
    case CAIRO_SURFACE_TYPE_SCRIPT:         return new_Surface(L, LUACAIRO ".ScriptSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_QT:             return new_Surface(L, LUACAIRO ".QtSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_RECORDING:      return new_Surface(L, LUACAIRO ".RecordingSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_VG:             return new_Surface(L, LUACAIRO ".VgSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_GL:             return new_Surface(L, LUACAIRO ".GlSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_DRM:            return new_Surface(L, LUACAIRO ".DrmSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_TEE:            return new_Surface(L, LUACAIRO ".TeeSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_XML:            return new_Surface(L, LUACAIRO ".XmlSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_SKIA:           return new_Surface(L, LUACAIRO ".SkiaSurface.mt", cs, typecs, havecs);
    case CAIRO_SURFACE_TYPE_SUBSURFACE:     return new_Surface(L, LUACAIRO ".SubSurface.mt", cs, typecs, havecs);
#endif
    }

    return 0;
}

static int gc_Surface (lua_State *L)
{
    Surface *o = check_Surface_ud(L, 1);

    if (o->cs_ && o->havecs_)
    {
        cairo_surface_destroy(o->cs_);
        o->cs_     = NULL;
        o->havecs_ = 0;
        o->typecs_ = 0;
    }

    return 0;
}

static int tostring_Surface (lua_State *L)
{
    Surface *o = check_Surface_ud(L, 1);
    lua_pushfstring(L, "Surface (%p), cairo_surface_t (%p), Type:%d", (void*)o, (void*)o->cs_, o->typecs_);
    return 1;
}



static int luaopen_lsurface (lua_State *L)
{
    return 0;
}

#endif  /* LUACAIRO_HAS_OO_INTERFACE */




static int luaopen_lcairo_ud_surface (lua_State* L)
{
#if LUACAIRO_HAS_OO_INTERFACE

    luaopen_lsurface (L);

#endif  /* LUACAIRO_HAS_OO_INTERFACE */
    return 0;
}
