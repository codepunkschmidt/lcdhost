TARGET = LH_Bar
TEMPLATE = lib
DEFINES += LH_BAR_LIBRARY

include(../Plugins.pri)
include(../CF/CF.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_QtCPU.cpp \
    ../LH_QtNetwork.cpp \
    LH_QtPlugin_Bar.cpp \
    LH_Bar.cpp \
    LH_BarNetIn.cpp \
    LH_BarNetOut.cpp \
    LH_BarMemPhysical.cpp \
    LH_BarMemVirtual.cpp \
    LH_BarCPUAverage.cpp \
    LH_BarCPUHistogram.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_QtCPU.h \
    ../LH_QtNetwork.h \
    LH_QtPlugin_Bar.h \
    LH_Bar.h
