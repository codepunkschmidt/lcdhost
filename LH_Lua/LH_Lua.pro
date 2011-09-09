TARGET = LH_Lua
TEMPLATE = lib
DEFINES += LH_LUA_LIBRARY

load(../PluginsConfig.prf)

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

SOURCES += \
    LH_QtPlugin_Lua.cpp \
    LH_LuaClass.cpp \
    LH_LuaInstance.cpp \
    luaXT.cpp \
    LH_LuaSetupItem.cpp \
    LH_LuaThread.cpp \
    LH_Lua.cpp \
    LH_LuaVariant.cpp

INCLUDEPATH += $$PWD

HEADERS += \
    LH_QtPlugin_Lua.h \
    LH_LuaClass.h \
    LH_LuaInstance.h \
    LH_LuaSetupItem.h \
    LH_LuaThread.h \
    LH_Lua.h \
    LH_LuaVariant.h


macx {
    # Uglyhack until Qt fixes CGFontRef export: http://bugreports.qt.nokia.com/browse/QTBUG-17890
    QTBASE = $$replace(QMAKE_QMAKE,/gcc/bin/qmake,/../../..)
    !exists( $$QTBASE/QtSources ): error(QTBASE is not correctly set or Qt sources not installed)
    QTSRC = $$quote($$QTBASE/QtSources/$$QT_VERSION/src)
    !exists( $$QTSRC/corelib ): error(QTSRC is not correctly set or Qt sources not installed)
    INCLUDEPATH += \
        $$QTSRC/3rdparty/harfbuzz/src \
        $$QTSRC/gui/text \
        $$QTSRC/corelib/kernel \
        $$QTSRC/corelib/tools \
        $$QTSRC/gui/painting \
        $$QTSRC/gui/kernel
}

DEFINES += LUA_STATIC GD_FREETYPE GD_PNG GD_GIF NONDLL HAVE_LIBZ HAVE_LIBPNG HAVE_LIBFREETYPE HAVE_FT2BUILD_H

# libz
Z_DIR = zlib-1.2.5
INCLUDEPATH += $$Z_DIR
VPATH += $$Z_DIR
SOURCES += adler32.c compress.c crc32.c deflate.c gzclose.c gzlib.c gzread.c \
    gzwrite.c infback.c inffast.c inflate.c inftrees.c trees.c uncompr.c zutil.c

# Lua
LUA_DIR = lua-5.1.4/src
INCLUDEPATH += $$LUA_DIR
VPATH += $$LUA_DIR
SOURCES += lapi.c lauxlib.c lbaselib.c lcode.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c \
   lgc.c linit.c liolib.c llex.c lua.c lmathlib.c lmem.c loadlib.c lobject.c \
   lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c \
   ltm.c lundump.c lvm.c lzio.c

# pixman
PIXMAN_DIR = pixman-0.21.4
INCLUDEPATH += $$PIXMAN_DIR
VPATH += $$PIXMAN_DIR
DEFINES += PACKAGE=pixman-1 PACKAGE_VERSION PACKAGE_BUGREPORT USE_MMX USE_SSE2
QMAKE_CFLAGS_RELEASE += -mmmx -msse2 -Winline
QMAKE_CFLAGS_DEBUG += -mmmx -msse2 -Winline
#macx:DEFINES += HAVE_PTHREAD_SETSPECIFIC
unix:DEFINES += HAVE_PTHREAD_SETSPECIFIC

HEADERS += \
    pixman.h pixman-accessor.h pixman-combine32.h pixman-combine64.h \
    pixman-compiler.h pixman-fast-path.h pixman-edge-imp.h pixman-private.h \
    pixman-version.h
SOURCES += \
    pixman.c pixman-image.c pixman-utils.c pixman-access-accessors.c \
    pixman-access-normal.c pixman-region16.c pixman-region32.c \
    pixman-combine32.c  pixman-combine64.c pixman-edge-accessors.c  \
    pixman-edge-normal.c pixman-trap.c  pixman-timer.c pixman-matrix.c \
    pixman-gradient-walker.c pixman-linear-gradient.c \
    pixman-conical-gradient.c pixman-radial-gradient.c pixman-bits-image.c \
    pixman-cpu.c pixman-fast-path.c pixman-implementation.c \
    pixman-solid-fill.c pixman-general.c \
    pixman-mmx.c pixman-sse2.c

# lpng
PNG_DIR = lpng-1.5.1
INCLUDEPATH += $$PNG_DIR
VPATH += $$PNG_DIR
SOURCES += png.c pngerror.c pngget.c pngmem.c pngpread.c \
    pngread.c pngrio.c pngrtran.c pngrutil.c pngset.c \
    pngtrans.c pngwio.c pngwrite.c pngwtran.c pngwutil.c

# cairo
CAIRO_DIR = cairo-1.10.2
INCLUDEPATH += $$CAIRO_DIR
VPATH += $$CAIRO_DIR
DEFINES += \
    HAVE_STDINT_H \
    CAIRO_HAS_XLIB_SURFACE=0 \
    CAIRO_HAS_XLIB_XRENDER_SURFACE=0 \
    CAIRO_HAS_XCB_SURFACE=0 \
    CAIRO_HAS_XLIB_XCB_FUNCTIONS=0 \
    CAIRO_HAS_XCB_SHM_FUNCTIONS=0 \
    CAIRO_HAS_QT_SURFACE=0 \
    CAIRO_HAS_SKIA_SURFACE=0 \
    CAIRO_HAS_OS2_SURFACE=0 \
    CAIRO_HAS_BEOS_SURFACE=0 \
    CAIRO_HAS_DRM_SURFACE=0 \
    CAIRO_HAS_DRM_XR_FUNCTIONS=0 \
    CAIRO_HAS_GALLIUM_SURFACE=0 \
    CAIRO_HAS_XCB_DRM_FUNCTIONS=0 \
    CAIRO_HAS_PNG_FUNCTIONS=1 \
    CAIRO_HAS_GL_SURFACE=0 \
    CAIRO_HAS_DIRECTFB_SURFACE=0 \
    CAIRO_HAS_VG_SURFACE=0 \
    CAIRO_HAS_EGL_FUNCTIONS=0 \
    CAIRO_HAS_GLX_FUNCTIONS=0 \
    CAIRO_HAS_WGL_FUNCTIONS=0 \
    CAIRO_HAS_SCRIPT_SURFACE=0 \
    CAIRO_HAS_PS_SURFACE=0 \
    CAIRO_HAS_PDF_SURFACE=0 \
    CAIRO_HAS_SVG_SURFACE=0 \
    CAIRO_HAS_TEST_SURFACES=0 \
    CAIRO_HAS_TEE_SURFACE=0 \
    CAIRO_HAS_XML_SURFACE=0 \
    CAIRO_HAS_GOBJECT_FUNCTIONS=0 \
    CAIRO_HAS_TRACE=0 \
    CAIRO_HAS_INTERPRETER=1 \
    CAIRO_HAS_SYMBOL_LOOKUP=0

HEADERS += $$CAIRO_DIR/cairo.h $$CAIRO_DIR/cairo-deprecated.h cairo-version.h
SOURCES += \
    cairo-analysis-surface.c cairo-arc.c cairo-array.c cairo-atomic.c \
    cairo-base64-stream.c cairo-base85-stream.c cairo-bentley-ottmann.c \
    cairo-bentley-ottmann-rectangular.c cairo-bentley-ottmann-rectilinear.c \
    cairo-botor-scan-converter.c cairo-boxes.c cairo.c cairo-cache.c \
    cairo-clip.c cairo-color.c cairo-composite-rectangles.c cairo-debug.c \
    cairo-device.c cairo-fixed.c cairo-font-face.c cairo-font-face-twin.c \
    cairo-font-face-twin-data.c cairo-font-options.c cairo-freelist.c \
    cairo-freed-pool.c cairo-gstate.c cairo-hash.c cairo-hull.c cairo-image-info.c \
    cairo-image-surface.c cairo-lzw.c cairo-matrix.c cairo-recording-surface.c \
    cairo-misc.c cairo-mutex.c cairo-observer.c cairo-output-stream.c \
    cairo-paginated-surface.c cairo-path-bounds.c cairo-path.c \
    cairo-path-fill.c cairo-path-fixed.c cairo-path-in-fill.c \
    cairo-path-stroke.c cairo-pattern.c cairo-pen.c cairo-polygon.c \
    cairo-rectangle.c cairo-rectangular-scan-converter.c cairo-region.c \
    cairo-rtree.c cairo-scaled-font.c cairo-scaled-font-subsets.c \
    cairo-slope.c cairo-spans.c cairo-truetype-subset.c \
    cairo-spline.c cairo-stroke-style.c cairo-surface.c cairo-png.c \
    cairo-surface-fallback.c cairo-surface-clipper.c cairo-surface-offset.c \
    cairo-surface-snapshot.c cairo-surface-subsurface.c cairo-surface-wrapper.c \
    cairo-tor-scan-converter.c cairo-toy-font-face.c cairo-traps.c \
    cairo-unicode.c cairo-user-font.c cairo-version.c cairo-wideint.c

win32 {
    SOURCES += cairo-win32-font.c cairo-win32-surface.c cairo-win32-printing-surface.c
    DEFINES += CAIRO_HAS_WIN32_SURFACE=1 CAIRO_HAS_WIN32_FONT=1
    DEFINES += CAIRO_HAS_QUARTZ_SURFACE=0 CAIRO_HAS_QUARTZ_FONT=0 CAIRO_HAS_QUARTZ_IMAGE_SURFACE=0
    DEFINES += CAIRO_HAS_FT_FONT=0 CAIRO_HAS_FC_FONT=0
    DEFINES += CAIRO_HAS_PTHREAD=0
    LIBS += -lgdi32 -lmsimg32
}

macx {
    SOURCES += cairo-quartz-font.c cairo-quartz-surface.c cairo-quartz-image-surface.c
    DEFINES += CAIRO_HAS_WIN32_SURFACE=0 CAIRO_HAS_WIN32_FONT=0
    DEFINES += CAIRO_HAS_QUARTZ_SURFACE=1 CAIRO_HAS_QUARTZ_FONT=1 CAIRO_HAS_QUARTZ_IMAGE_SURFACE=1
    DEFINES += CAIRO_HAS_FT_FONT=0 CAIRO_HAS_FC_FONT=0
    DEFINES += CAIRO_HAS_PTHREAD=1
    LIBS += -framework Carbon -framework CoreFoundation -framework Cocoa
}

unix:!macx {
    SOURCES += cairo-ft-font.c
    INCLUDEPATH += /usr/include/freetype2
    DEFINES += CAIRO_HAS_WIN32_SURFACE=0 CAIRO_HAS_WIN32_FONT=0
    DEFINES += CAIRO_HAS_QUARTZ_SURFACE=0 CAIRO_HAS_QUARTZ_FONT=0 CAIRO_HAS_QUARTZ_IMAGE_SURFACE=0
    DEFINES += CAIRO_HAS_FT_FONT=1 CAIRO_HAS_FC_FONT=0
    DEFINES += CAIRO_HAS_PTHREAD=1
}

# Lua cairo
SOURCES += lua-cairo/lcairo.c


