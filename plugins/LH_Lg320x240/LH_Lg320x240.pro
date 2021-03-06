TARGET = LH_Lg320x240
TEMPLATE = lib
DEFINES += LG320x240_LIBRARY
CONFIG += device usb wow64

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_Lg320x240.cpp \
    Lg320x240Device.cpp \
    LogitechG19Thread.cpp \
    LogitechG19.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_Lg320x240.h \
    Lg320x240Device.h \
    LogitechG19Thread.h \
    LogitechG19.h
