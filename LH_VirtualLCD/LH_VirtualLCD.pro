TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY

include(../linkdata/LCDHost.pri)

SOURCES += \
    LH_VirtualLCD.cpp

HEADERS += \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
