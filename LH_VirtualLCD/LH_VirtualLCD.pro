TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY

include(../Plugins.pri)

SOURCES += $$PLUGIN_SOURCES \
    ../LH_QtOutputDevice.cpp \
    LH_VirtualLCD.cpp

HEADERS += $$PLUGIN_HEADERS \
    ../LH_QtOutputDevice.h \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
