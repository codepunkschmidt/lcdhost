TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY

load(../PluginsConfig.prf)

SOURCES += \
    LH_VirtualLCD.cpp

HEADERS += \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
