TARGET = LH_Lg160x43
TEMPLATE = lib
DEFINES += LG160X43_LIBRARY
CONFIG += device

include(../Plugins.pri)
include(../HID.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../wow64.cpp \
    LH_Lg160x43.cpp \
    Lg160x43Device.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../wow64.h \
    LH_Lg160x43.h \
    Lg160x43Device.h
