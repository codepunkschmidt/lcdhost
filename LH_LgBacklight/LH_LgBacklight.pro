TARGET = LH_LgBacklight
TEMPLATE = lib
DEFINES += LGBACKLIGHT_LIBRARY

include(../linkdata/LCDHost.pri)
include(../linkdata/hidapi.pri)

SOURCES += \
    LH_LgBacklight.cpp \
    LgBacklightDevice.cpp

HEADERS += \
    LH_LgBacklight.h\
    LgBacklightDevice.h

RESOURCES += \
    resources.qrc
