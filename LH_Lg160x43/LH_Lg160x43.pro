TARGET = LH_Lg160x43
TEMPLATE = lib
DEFINES += LG160X43_LIBRARY
CONFIG += hidapi

load(../PluginsConfig.prf)

SOURCES += \
    LH_Lg160x43.cpp \
    Lg160x43Device.cpp

HEADERS += \
    LH_Lg160x43.h \
    Lg160x43Device.h
