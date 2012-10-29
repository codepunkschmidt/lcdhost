TARGET = LH_Bar
TEMPLATE = lib
DEFINES += LH_BAR_LIBRARY
CONFIG += cf systemstats

include(../Plugins.pri)

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_Bar.h \
    LH_Bar.h

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Bar.cpp \
    LH_Bar.cpp \
    LH_BarNetIn.cpp \
    LH_BarNetOut.cpp \
    LH_BarMemPhysical.cpp \
    LH_BarMemVirtual.cpp \
    LH_BarCPUAverage.cpp \
    LH_BarCPUHistogram.cpp
