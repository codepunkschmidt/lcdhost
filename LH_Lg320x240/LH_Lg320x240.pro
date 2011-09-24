TARGET = LH_Lg320x240
TEMPLATE = lib
DEFINES += LG320x240_LIBRARY
CONFIG += lh_plugin libusb

load(../PluginsConfig.prf)

SOURCES += \
    LH_Lg320x240.cpp \
    Lg320x240Device.cpp \
    LogitechG19Thread.cpp \
    LogitechG19.cpp

HEADERS += \
    LH_Lg320x240.h \
    Lg320x240Device.h \
    LogitechG19Thread.h \
    LogitechG19.h
