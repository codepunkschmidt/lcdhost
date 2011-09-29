TARGET = LH_LgBacklight
TEMPLATE = lib
DEFINES += LGBACKLIGHT_LIBRARY
CONFIG += lh_api5 hidapi

load(../PluginsConfig.prf)

SOURCES += \
    LH_LgBacklight.cpp \
    LgBacklightDevice.cpp

HEADERS += \
    LH_LgBacklight.h\
    LgBacklightDevice.h

RESOURCES += \
    resources.qrc
