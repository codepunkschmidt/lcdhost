TARGET = LH_Dial
TEMPLATE = lib
DEFINES += LH_DIAL_LIBRARY
CONFIG += systemstats

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Dial.cpp \
    LH_Dial.cpp \
    LH_DialCPUAverage.cpp \
    LH_DialCPUHistogram.cpp \
    LH_DialNetOut.cpp \
    LH_DialNetIn.cpp \
    LH_DialMemVirtual.cpp \
    LH_DialMemPhysical.cpp \
    LH_DialTime.cpp \
    LH_DialTest.cpp \
    LH_PieCPUAverage.cpp \
    LH_PieCPUHistogram.cpp \
    LH_PieMem.cpp
	
HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_Dial.h \
    LH_Dial.h \
    LH_DialTime.h



