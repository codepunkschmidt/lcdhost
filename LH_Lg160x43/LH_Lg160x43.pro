TARGET = LH_Lg160x43
TEMPLATE = lib
DEFINES += LG160X43_LIBRARY

include(../Plugins.pri)
include(../HID.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtDevice.cpp \
    ../wow64.cpp \
    LH_Lg160x43.cpp \
    Lg160x43Device.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtInstance.h \
    ../LH_QtSetupItem.h \
    ../LH_QtDevice.h \
    ../wow64.h \
    LH_Lg160x43.h \
    Lg160x43Device.h
