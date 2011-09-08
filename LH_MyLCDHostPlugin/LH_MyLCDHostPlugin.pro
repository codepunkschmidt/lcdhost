#-------------------------------------------------
#
# Project created by QtCreator 2011-09-07T09:42:40
#
#-------------------------------------------------

TARGET = LH_MyLCDHostPlugin
TEMPLATE = lib

DEFINES += LH_MYLCDHOSTPLUGIN_LIBRARY

include(../linkdata/LCDHost.pri)

SOURCES += LH_MyLCDHostPlugin.cpp \
    LH_QtPlugin_MyLCDHostPlugin.cpp

HEADERS += LH_MyLCDHostPlugin.h\
        LH_MyLCDHostPlugin_global.h \
    LH_QtPlugin_MyLCDHostPlugin.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE3D59779
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = LH_MyLCDHostPlugin.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}


