#
# lcdhost.pri
#
# You can set LH_DIR_INSTALL to your LCDHost installation directory if needed.
#

isEmpty(LH_DIR_SOURCES): error($$_PRO_FILE_: lcdhost.pri: LH_DIR_SOURCES not set)
!exists($$LH_DIR_SOURCES/lcdhost.pro): error($$_PRO_FILE_: lcdhost.pri: missing \"$$LH_DIR_SOURCES/lcdhost.pro\")
!isEqual(LH_DIR_SOURCES, $$dirname(LH_DIR_FEATURES)) {
    error($$_PRO_FILE_: lcdhost.pri: \"$$LH_DIR_SOURCES\" != \"$$dirname(LH_DIR_FEATURES)\")
}

exists($(HOME)/.ccache): *-g++*: {
    QMAKE_CXX = $$quote(ccache $${QMAKE_CXX})
    QMAKE_CC = $$quote(ccache $${QMAKE_CC})
}

defineReplace(lh_sanitizepath) {
    path = $$1
    win32-msvc* {
        components = $$split(path, :)
        count(components, 2): path = $$lower($$first(components)):$$last(components)
        return($$replace(path, \\\\, /))
    } else {
        return($$path)
    }
}

defineReplace(lh_systempath) {
    return($$replace(1, [/\\\\], $${QMAKE_DIR_SEP}))
}

defineTest(lh_mkdir) {
    win32: system($${QMAKE_MKDIR} \"$$1\" 2>NUL || echo>NUL)
    else: system($${QMAKE_MKDIR} \"$$1\")
    return(true)
}

defineTest(lh_copy) {
    win32: system($${QMAKE_COPY} \"$$1\" \"$$2\" >NUL)
    else: system($${QMAKE_COPY} \"$$1\" \"$$2\")
    return(true)
}

defineTest(lh_qmakevars) {
    isEmpty(LH_DIR_SOURCES): error(LH_DIR_SOURCES is empty)
    isEmpty(LH_RELDEB) {
        CONFIG(debug, debug|release): LH_RELDEB=debug
        else: LH_RELDEB=release
        export(LH_RELDEB)
    }
    isEmpty(LH_DIR_LIB) {
        LH_DIR_LIB=$$quote($${LH_DIR_SOURCES}/lib/$${LH_RELDEB})
        export(LH_DIR_LIB)
    }
    isEmpty(LH_DIR_INCLUDE) {
        LH_DIR_INCLUDE=$$quote($${LH_DIR_SOURCES}/include)
        export(LH_DIR_INCLUDE)
    }
    isEmpty(LH_DIR_LINKDATA) {
        LH_DIR_LINKDATA=$$quote($${LH_DIR_SOURCES}/linkdata)
        export(LH_DIR_LINKDATA)
    }
    isEmpty(LH_DIR_CODELEAP) {
        LH_DIR_CODELEAP=$$quote($${LH_DIR_SOURCES}/codeleap)
        export(LH_DIR_CODELEAP)
    }
    isEmpty(LH_DIR_TOP) {
        LH_DIR_TOP=$$quote($$dirname(LH_DIR_SOURCES))
        export(LH_DIR_TOP)
    }
    isEmpty(LH_DIR_INSTALL) {
        LH_DIR_INSTALL=$$quote($${LH_DIR_TOP}/$${LH_RELDEB})
        export(LH_DIR_INSTALL)
    }
    isEmpty(LH_DIR_BINARIES) {
        macx: LH_DIR_BINARIES=$$LH_DIR_INSTALL/LCDHost.app/Contents/MacOS
        else: LH_DIR_BINARIES=$$LH_DIR_INSTALL
        export(LH_DIR_BINARIES)
    }
    isEmpty(LH_DIR_PLUGINS) {
        macx: LH_DIR_PLUGINS=$$LH_DIR_INSTALL/LCDHost.app/Contents/PlugIns
        else: LH_DIR_PLUGINS=$$LH_DIR_INSTALL/plugins
        export(LH_DIR_PLUGINS)
    }
    isEmpty(LH_DIR_LAYOUTS) {
        macx: LH_DIR_LAYOUTS=$$LH_DIR_INSTALL/LCDHost.app/Contents/Resources/layouts
        else: LH_DIR_LAYOUTS=$$LH_DIR_INSTALL/layouts
        export(LH_DIR_LAYOUTS)
    }
    isEmpty(LH_DESTDIR): contains(TEMPLATE, app|lib) {
        LH_DESTDIR=$$LH_DIR_BINARIES
        contains(TEMPLATE, lib) {
            lh_plugin: LH_DESTDIR=$$LH_DIR_PLUGINS
            staticlib: LH_DESTDIR=$$LH_DIR_LIB
        } else {
            macx: app_bundle: LH_DESTDIR=$$LH_DIR_INSTALL
        }
        export(LH_DESTDIR)
    }
    return(true)
}

defineReplace(lh_destdir) {
    lh_qmakevars()
    isEmpty(DESTDIR): !isEmpty(LH_DESTDIR) {
        DESTDIR=$$LH_DESTDIR
        export(DESTDIR)
    }
    contains(TEMPLATE, app|lib) {
        contains(TEMPLATE, app) {
            unix {
                macx: QMAKE_LFLAGS+=-Wl,-rpath,@loader_path,-rpath,@loader_path/../Frameworks
                else: QMAKE_LFLAGS+='-Wl,-rpath,\'$$ORIGIN\''
                export(QMAKE_LFLAGS)
            }
        }
        contains(TEMPLATE, lib) {
            macx {
                lh_plugin: QMAKE_LFLAGS_SONAME=-Wl,-install_name,@executable_path/../PlugIns/
                else: QMAKE_LFLAGS_SONAME=-Wl,-install_name,@executable_path/
                export(QMAKE_LFLAGS_SONAME)
            }
        }

        lh_changeset_revision()
        LIBS*=$$quote(-L$$LH_DIR_BINARIES)
        LIBS*=$$quote(-L$$LH_DIR_LIB)
        INCLUDEPATH*=$$quote($$LH_DIR_INCLUDE)
        INCLUDEPATH*=$$quote($$LH_DIR_CODELEAP)

        win32 {
            win32-msvc2010: DEFINES*=_CRT_SECURE_NO_WARNINGS
        }

        for(lh_feature, LH_FEATURES) {
            contains(CONFIG, $${lh_feature}): eval($${lh_feature}())
        }

        greaterThan(QT_MAJOR_VERSION, 4) {
            contains(QT, gui): QT*=widgets
            contains(QT, webkit): QT*=webkitwidgets
        }

        export(CONFIG)
        export(QT)
        export(DEFINES)
        export(LIBS)
        export(INCLUDEPATH)
    }

    return($$LH_DESTDIR)
}
