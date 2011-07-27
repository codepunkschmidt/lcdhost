TARGET = LH_LCoreReboot
TEMPLATE = lib
DEFINES += LH_LCOREREBOOT_LIBRARY

win32:LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib" -lpsapi

include(../Plugins.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtInstance.cpp \
    LH_QtPlugin_LCoreReboot.cpp

HEADERS += \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtInstance.h \
    LH_QtPlugin_LCoreReboot.h

