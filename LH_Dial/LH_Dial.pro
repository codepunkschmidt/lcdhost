TARGET = LH_Dial
TEMPLATE = lib
DEFINES += LH_DIAL_LIBRARY

include(../Plugins.pri)

SOURCES += $$PLUGIN_SOURCES \
    ../LH_QtCPU.cpp \
    ../LH_QtNetwork.cpp \
    ../LH_QtMemory.cpp \
    ../LH_Qt_array_int_ui.cpp \
    ../LH_Qt_array_string_ui.cpp \
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

HEADERS += $$PLUGIN_HEADERS \
    ../LH_QtCPU.h \
    ../LH_QtNetwork.h \
    ../LH_QtMemory.h \
    ../LH_Qt_array_int_ui.h \
    ../LH_Qt_array_string.h \
    ../LH_Qt_array.h \
    ../LH_Qt_array_double.h \
    ../LH_Qt_array_int.h \
    ../LH_Qt_array_string_ui.h \
    LH_QtPlugin_Dial.h \
    LH_Dial.h \
    LH_DialTime.h
