TARGET = LH_Decor
TEMPLATE = lib
DEFINES += LH_DECOR_LIBRARY

include(../linkdata/LCDHost.pri)
include(../codeleap/ConditionalFormatting.pri)

SOURCES += \
    LH_QtPlugin_Decor.cpp \
    LH_Rectangle.cpp \
    LH_Background.cpp

HEADERS +=\
    LH_QtPlugin_Decor.h \
    LH_Rectangle.h \
    LH_Background.h \
    LH_StaticRectangle.h
