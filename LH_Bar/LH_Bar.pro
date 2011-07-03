TARGET = LH_Bar
TEMPLATE = lib
DEFINES += LH_BAR_LIBRARY

include(../Plugins.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
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
    ../LH_QtPlugin.h \
    ../LH_QtInstance.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtCPU.h \
    ../LH_QtNetwork.h \
    LH_QtPlugin_Bar.h \
    LH_Bar.h
