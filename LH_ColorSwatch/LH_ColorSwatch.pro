TARGET = LH_ColorSwatch
TEMPLATE = lib
DEFINES += VERSION=1.00
CONFIG += cf

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Decor/LH_Rectangle.cpp \
    LH_QtPlugin_ColorSwatch.cpp \
    LH_ColorSwatch.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Decor/LH_Rectangle.h \
    LH_QtPlugin_ColorSwatch.h \
    LH_ColorSwatch.h
