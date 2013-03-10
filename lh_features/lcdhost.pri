#
# lcdhost.pri
#
# You can set LH_DIR_INSTALL to your LCDHost installation directory if needed.
#

isEmpty(LH_DIR_SOURCES): error($$_PRO_FILE_: lcdhost.prf: LH_DIR_SOURCES not set)
LH_DIR_FEATURES=$$quote($$PWD)
LH_DIR_LINKDATA=$$quote($$LH_DIR_SOURCES/linkdata)
LH_DIR_CODELEAP=$$quote($$LH_DIR_SOURCES/codeleap)

!exists($$LH_DIR_SOURCES/lcdhost.pro): error($$_PRO_FILE_: lcdhost.prf: missing \"$$LH_DIR_SOURCES/lcdhost.pro\")
!isEqual(LH_DIR_SOURCES, $$dirname(LH_DIR_FEATURES)) {
    error($$_PRO_FILE_: lcdhost.prf: \"$$LH_DIR_SOURCES\" != \"$$dirname(LH_DIR_FEATURES)\")
}

exists($(HOME)/.ccache): *-g++*: {
    QMAKE_CXX = $$quote(ccache $${QMAKE_CXX})
    QMAKE_CC = $$quote(ccache $${QMAKE_CC})
}

defineTest(lh_changeset_revision) {
    hg_output=$$system(hg log -l 1 --template {node}/{rev} \"$$PWD/$$1\")
    LH_CHANGESET=$$section(hg_output,"/",0,0)
    isEmpty(LH_CHANGESET): return(false)
    LH_REVISION=$$section(hg_output,"/",1,1)
    isEmpty(LH_REVISION): LH_REVISION=1
    export(LH_CHANGESET)
    export(LH_REVISION)
    return(true)
}

defineTest(lh_log_config) {
    cfgtext += $$quote(TEMPLATE=$$TEMPLATE)
    cfgtext += $$quote(CONFIG=$$CONFIG)
    cfgtext += $$quote(QT=$$QT)
    cfgtext += $$quote(LH_DIR_INSTALL=$$LH_DIR_INSTALL)
    cfgtext += $$quote(LH_DIR_BINARIES=$$LH_DIR_BINARIES)
    cfgtext += $$quote(LH_DIR_PLUGINS=$$LH_DIR_PLUGINS)
    !isEmpty(TARGET): cfgtext += $$quote(TARGET=$$TARGET)
    !isEmpty(DESTDIR): cfgtext += $$quote(DESTDIR=$$DESTDIR)
    for(x, DEFINES): cfgtext += $$quote(DEFINES: \"$$x\")
    for(x, INCLUDEPATH): cfgtext += $$quote(INCLUDEPATH: \"$$x\")
    for(x, LIBS): cfgtext += $$quote(LIBS: \"$$x\")
    for(x, INSTALLS): cfgtext += $$quote(INSTALLS: \"$$x\")

    cfgsuffix=$$quote($$1)
    isEmpty(cfgsuffix): cfgsuffix=.config
    else: cfgsuffix=$$quote(.$${cfgsuffix}.config)
    cfgfile=$$quote($${_PRO_FILE_}$${cfgsuffix})
    exists($$cfgfile) {
        system($$QMAKE_DEL_FILE \"$$cfgfile\")
    }
    for(x, cfgtext) {
        win32: system(echo $$quote($$x)>>\"$$cfgfile\")
        else: system(echo \'$$quote($$x)\'>>\"$$cfgfile\")
    }
}

defineReplace(lh_destdir) {
    !isEmpty(LH_DIR_TOP) {
        warning($$_PRO_FILE_: lh_destdir() already called: DESTDIR is \"$$DESTDIR\")
        return($$DESTDIR)
    }

    LH_DIR_TOP=$$dirname(LH_DIR_SOURCES)
    isEmpty(LH_DIR_TOP): error($$_PRO_FILE_: lcdhost.prf: LH_DIR_TOP is empty)

    isEmpty(LH_DIR_INSTALL) {
        CONFIG(debug, debug|release): LH_DIR_INSTALL=$$LH_DIR_TOP/debug
        else: LH_DIR_INSTALL=$$LH_DIR_TOP/release
    }

    macx {
        LH_DIR_BINARIES=$$LH_DIR_INSTALL/LCDHost.app/Contents/MacOS
        LH_DIR_PLUGINS=$$LH_DIR_INSTALL/LCDHost.app/Contents/PlugIns
        LH_DIR_LAYOUTS=$$LH_DIR_INSTALL/LCDHost.app/Contents/Resources/layouts
    } else {
        LH_DIR_BINARIES=$$LH_DIR_INSTALL
        LH_DIR_PLUGINS=$$LH_DIR_INSTALL/plugins
        LH_DIR_LAYOUTS=$$LH_DIR_INSTALL/layouts
    }

    contains(TEMPLATE, app) {
        DESTDIR=$$quote($$LH_DIR_BINARIES)
        macx {
            app_bundle: DESTDIR=$$quote($$LH_DIR_INSTALL)
            QMAKE_LFLAGS += -Wl,-rpath,@loader_path,-rpath,@loader_path/../Frameworks
        }
        unix:!macx: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
    }

    contains(TEMPLATE, lib) {
        lh_plugin: DESTDIR=$$quote($$LH_DIR_PLUGINS)
        else: DESTDIR=$$quote($$LH_DIR_BINARIES)
        macx {
            lh_plugin: QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/../PlugIns/
            else: QMAKE_LFLAGS_SONAME = -Wl,-install_name,@executable_path/
        }
    }

    !isEmpty(DESTDIR): lh_changeset_revision() {
        # message($$_PRO_FILE_: CHANGESET=\"$$LH_CHANGESET\")
        # DEFINES*=CHANGESET=\"\\\"$$LH_CHANGESET\\\"\"
        DEFINES*=$$quote(CHANGESET=\\\"$$LH_CHANGESET\\\")
        DEFINES*=$$quote(REVISION=$$LH_REVISION)
    }

    win32 {
        win32-msvc2010: DEFINES *= _CRT_SECURE_NO_WARNINGS
    }

    LIBS *= $$quote(-L$$LH_DIR_BINARIES)
    INCLUDEPATH *= $$quote($$LH_DIR_LINKDATA)
    INCLUDEPATH *= $$quote($$LH_DIR_CODELEAP)

    LH_FEATURES=$$files($$LH_DIR_FEATURES/*.pri)
    for(featfile, LH_FEATURES) {
        LH_FEATURE=$$basename(featfile)
        LH_FEATURE~=s/\\.pri//
        contains(CONFIG, $$LH_FEATURE): include($$featfile)
    }

    greaterThan(QT_MAJOR_VERSION, 4) {
        contains(QT, gui): QT *= widgets
        contains(QT, webkit): QT *= webkitwidgets
    }

    export(QT)
    export(DESTDIR)
    export(LIBS)
    export(DEFINES)
    export(QMAKE_LFLAGS_SONAME)
    export(QMAKE_LFLAGS)
    export(INCLUDEPATH)
    export(LH_DIR_TOP)
    export(LH_DIR_INSTALL)
    export(LH_DIR_BINARIES)
    export(LH_DIR_PLUGINS)
    export(LH_DIR_LAYOUTS)

    return($$DESTDIR)
}
