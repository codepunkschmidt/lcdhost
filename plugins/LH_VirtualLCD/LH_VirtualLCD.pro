TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY
CONFIG += device

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_VirtualLCD.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
