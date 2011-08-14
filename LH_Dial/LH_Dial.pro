TARGET = LH_Dial
TEMPLATE = lib
DEFINES += LH_DIAL_LIBRARY

include(../Plugins.pri)

SOURCES += \
    LH_QtPlugin_Dial.cpp \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtCPU.cpp \
    ../LH_QtNetwork.cpp \
    ../LH_QtMemory.cpp \
    LH_Dial.cpp \
    LH_DialCPUAverage.cpp \
    LH_DialCPUHistogram.cpp \
    LH_DialNetOut.cpp \
    LH_DialNetIn.cpp \
    LH_DialMemVirtual.cpp \
    LH_DialMemPhysical.cpp \
    LH_DialTime.cpp \
    #LH_DialTest.cpp \
    ../LH_QtInstance.cpp \
    ../LH_Qt_array_int_ui.cpp \
    ../LH_Qt_array_string_ui.cpp
	
HEADERS += \
    LH_QtPlugin_Dial.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtCPU.h \
    ../LH_QtNetwork.h \
    ../LH_QtMemory.h \
    LH_Dial.h \
    LH_DialTime.h \
    ../LH_QtInstance.h \
    ../LH_Qt_array_int_ui.h \
    ../LH_Qt_array_string.h \
    ../LH_Qt_array.h \
    ../LH_Qt_array_double.h \
    ../LH_Qt_array_int.h \
    ../LH_Qt_array_string_ui.h
