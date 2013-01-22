TARGET = LH_LgBacklight
TEMPLATE = lib
DEFINES += LGBACKLIGHT_LIBRARY

include(../Plugins.pri)

LIBS *= -llh_hidapi

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_LgBacklight.cpp \
    LgBacklightDevice.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_LgBacklight.h\
    LgBacklightDevice.h

RESOURCES += \
    resources.qrc
