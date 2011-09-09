TARGET = LH_Bar
TEMPLATE = lib
DEFINES += LH_BAR_LIBRARY
CONFIG += conditionalformatting

load(../PluginsConfig.prf)
include(../codeleap/codeleap.pri)

SOURCES += \
    LH_QtPlugin_Bar.cpp \
    LH_Bar.cpp \
    LH_BarNetIn.cpp \
    LH_BarNetOut.cpp \
    LH_BarMemPhysical.cpp \
    LH_BarMemVirtual.cpp \
    LH_BarCPUAverage.cpp \
    LH_BarCPUHistogram.cpp

HEADERS += \
    LH_QtPlugin_Bar.h \
    LH_Bar.h
