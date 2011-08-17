TARGET = LH_LCoreReboot
TEMPLATE = lib
DEFINES += LH_LCOREREBOOT_LIBRARY

win32:LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib" -lpsapi

include(../Plugins.pri)

SOURCES += $$PLUGIN_SOURCES \
    LH_QtPlugin_LCoreReboot.cpp

HEADERS += $$PLUGIN_HEADERS \
    LH_QtPlugin_LCoreReboot.h

