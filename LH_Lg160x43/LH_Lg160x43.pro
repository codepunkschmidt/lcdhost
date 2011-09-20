TARGET = LH_Lg160x43
TEMPLATE = lib
DEFINES += LG160X43_LIBRARY

include(../Plugins.pri)
include(../HID.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_QtDevice.cpp \
    ../wow64.cpp \
    LH_Lg160x43.cpp \
    Lg160x43Device.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_QtDevice.h \
    ../wow64.h \
    LH_Lg160x43.h \
    Lg160x43Device.h
