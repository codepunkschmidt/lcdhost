TARGET = LH_VirtualLCD
TEMPLATE = lib
DEFINES += LH_VIRTUALLCD_LIBRARY

include(../linkdata/linkdata.pri)

SOURCES += \
    LH_VirtualLCD.cpp

HEADERS += \
    LH_VirtualLCD.h

RESOURCES += LH_VirtualLCD.qrc
