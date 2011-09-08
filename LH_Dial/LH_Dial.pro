TARGET = LH_Dial
TEMPLATE = lib
DEFINES += LH_DIAL_LIBRARY

include(../linkdata/LCDHost.pri)
include(../codeleap/codeleap_UI.pri)

SOURCES += \
    LH_QtPlugin_Dial.cpp \
    LH_Dial.cpp \
    LH_DialCPUAverage.cpp \
    LH_DialCPUHistogram.cpp \
    LH_DialNetOut.cpp \
    LH_DialNetIn.cpp \
    LH_DialMemVirtual.cpp \
    LH_DialMemPhysical.cpp \
    LH_DialTime.cpp
    #LH_DialTest.cpp \

HEADERS += \
    LH_QtPlugin_Dial.h \
    LH_Dial.h \
    LH_DialTime.h
