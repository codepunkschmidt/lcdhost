#
# Plugins.pri
#
# This plugin include file sets things up the way
# I want them when I build LCDHost. It may or may
# not work for you.
#
# In particular, the automated signing using
# QMAKE_POST_LINK need to use your own keys
# and URL's if you want to use it.
#

CONFIG(debug, debug|release):RELDEB = Debug
else:RELDEB = Release

CHANGESET_REVISION = $$system(hg log -l 1 --template {node}/{rev} $$PWD/$$TARGET)
CHANGESET = $$section(CHANGESET_REVISION,"/",0,0)
REVISION = $$section(CHANGESET_REVISION,"/",1,1)
isEmpty( REVISION ): REVISION = 0
DEFINES += CHANGESET=\"\\\"$$CHANGESET\\\"\"
DEFINES += REVISION=$$REVISION

win32 {
    LCDHOST_PLUGINS = $$PWD/../$$RELDEB/plugins
    LCDHOST_BINARIES = $$PWD/../$$RELDEB
}

macx {
    LCDHOST_PLUGINS = $$PWD/../$$RELDEB/LCDHost.app/Contents/PlugIns
    LCDHOST_BINARIES = $$PWD/../$$RELDEB/LCDHost.app/Contents/MacOS
}

unix:!macx {
    LCDHOST_PLUGINS = $$PWD/../$$RELDEB/plugins
    LCDHOST_BINARIES = $$PWD/../$$RELDEB
}

DESTDIR = $$LCDHOST_PLUGINS

PLUGIN_SOURCES = \
    $$PWD/lh_plugin.c \
    $$PWD/LH_Variant.cpp \
    $$PWD/LH_QtVariant.cpp \
    $$PWD/LH_QtObject.cpp \
    $$PWD/LH_QtPlugin.cpp \
    $$PWD/LH_QtLayoutClass.cpp \
    $$PWD/LH_QtInstance.cpp \
    $$PWD/LH_QtSetupItem.cpp

PLUGIN_HEADERS += \
    $$PWD/lh_plugin.h \
    $$PWD/LH_Variant.h \
    $$PWD/LH_QtVariant.h \
    $$PWD/LH_QtObject.h \
    $$PWD/LH_QtPlugin.h \
    $$PWD/LH_QtLayoutClass.h \
    $$PWD/LH_QtInstance.h \
    $$PWD/LH_QtSetupItem.h

contains( TEMPLATE, lib ) {
    exists($$PWD/../lcdhost-private.pem) {
        QMAKE_POST_LINK = \
            $$LCDHOST_BINARIES/SignPlugin -c -o \
            $$PWD/../lcdhost-private.pem \
            http://www.linkdata.se/downloads/software/lcdhost/lcdhost-public.pem \
            $$DESTDIR/$$TARGET
    }
    exists($$PWD/../triscopic-private.pem) {
        QMAKE_POST_LINK = \
            $$LCDHOST_BINARIES/SignPlugin -c -o \
            $$PWD/../triscopic-private.pem \
            http://www.codeleap.co.uk/lcdhost/public-key \
            $$DESTDIR/$$TARGET
    }
}

