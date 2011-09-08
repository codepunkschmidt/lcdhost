TARGET = LH_LCoreReboot
TEMPLATE = lib
DEFINES += LH_LCOREREBOOT_LIBRARY

include(../linkdata/linkdata.pri)

win32:SOURCES += LH_QtPlugin_LCoreReboot.cpp
win32:HEADERS += LH_QtPlugin_LCoreReboot.h
win32:LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib" -lpsapi

