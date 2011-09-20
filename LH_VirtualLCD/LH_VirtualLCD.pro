TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_QtDevice.cpp \
    LH_VirtualLCD.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_QtDevice.h \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
