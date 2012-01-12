TARGET = LH_Decor
TEMPLATE = lib
DEFINES += LH_DECOR_LIBRARY

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Decor.cpp \
    LH_Rectangle.cpp \
    LH_Background.cpp \
    LH_RectangleStatic.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_Decor.h \
    LH_Rectangle.h \
    LH_Background.h
