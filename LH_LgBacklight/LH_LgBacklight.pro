TARGET = LH_LgBacklight
TEMPLATE = lib
DEFINES += LGBACKLIGHT_LIBRARY

include(../Plugins.pri)
include(../HID.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtSetupItem.cpp \
    LH_LgBacklight.cpp \
    LgBacklightDevice.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtInstance.h \
    ../LH_QtSetupItem.h \
    LH_LgBacklight.h\
    LgBacklightDevice.h

RESOURCES += \
    resources.qrc
