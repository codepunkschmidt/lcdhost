TARGET = LH_LCoreReboot
TEMPLATE = lib
DEFINES += LH_LCOREREBOOT_LIBRARY

win32 {
    include(../linkdata/linkdata.pri)
    SOURCES += LH_QtPlugin_LCoreReboot.cpp
    HEADERS += LH_QtPlugin_LCoreReboot.h
    LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib" -lpsapi
}

