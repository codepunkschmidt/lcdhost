TARGET = LH_Decor
TEMPLATE = lib
DEFINES += LH_DECOR_LIBRARY

include(../Plugins.pri)
include(../CF/CF.pri)

SOURCES += \
    ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_QtPlugin_Decor.cpp \
    LH_Rectangle.cpp \
    LH_Background.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtInstance.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_QtPlugin_Decor.h \
    LH_Rectangle.h \
    LH_Background.h \
    LH_StaticRectangle.h
