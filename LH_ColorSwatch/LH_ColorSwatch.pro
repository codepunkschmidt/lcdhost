TARGET = LH_ColorSwatch
TEMPLATE = lib
DEFINES += VERSION=1.00
CONFIG += systemstats cf

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Decor/LH_Rectangle.cpp \
    LH_QtPlugin_ColorSwatch.cpp \
    LH_ColorSwatch.cpp \
    LH_ColorSwatchCPUAverage.cpp \
    LH_ColorSwatchMemPhysical.cpp \
    LH_ColorSwatchMemVirtual.cpp \
    LH_ColorSwatchNetIn.cpp \
    LH_ColorSwatchNetOut.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Decor/LH_Rectangle.h \
    LH_QtPlugin_ColorSwatch.h \
    LH_ColorSwatch.h
