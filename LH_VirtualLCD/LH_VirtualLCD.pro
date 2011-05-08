TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY

include(../Plugins.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtDevice.cpp \
    LH_VirtualLCD.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtDevice.h \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
