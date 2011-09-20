TARGET = LH_Lg320x240
TEMPLATE = lib
DEFINES += LG320x240_LIBRARY
CONFIG += device

include(../Plugins.pri)
include(../USB.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../wow64.cpp \
    LH_Lg320x240.cpp \
    Lg320x240Device.cpp \
    LogitechG19Thread.cpp \
    LogitechG19.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../wow64.h \
    LH_Lg320x240.h \
    Lg320x240Device.h \
    LogitechG19Thread.h \
    LogitechG19.h
