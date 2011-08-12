TARGET = LH_LgLcdMan
TEMPLATE = lib
DEFINES += LGLCDMAN_LIBRARY

include(../Plugins.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtOutputDevice.cpp \
    ../LH_QtInputDevice.cpp \
    ../wow64.cpp \
    LH_LgLcdMan.cpp \
    LH_LgLcdLegacyThread.cpp \
    LH_LgLcdCallbackThread.cpp \
    LH_LgLcdThread.cpp \
    LogitechOutputDevice.cpp \
    LogitechDeviceManager.cpp \
    LogitechLegacyManager.cpp \
    LogitechManager.cpp \
    LogitechLegacyDevice.cpp \
    LogitechDevice.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtInstance.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtOutputDevice.h \
    ../LH_QtInputDevice.h \
    ../wow64.h \
    LH_LgLcdMan.h \
    EventLgLcdNotification.h \
    LH_LgLcdLegacyThread.h \
    EventLgLcdButton.h \
    LH_LgLcdCallbackThread.h \
    LH_LgLcdThread.h \
    LogitechOutputDevice.h \
    LogitechDeviceManager.h \
    LogitechLegacyManager.h \
    LogitechManager.h \
    LogitechLegacyDevice.h \
    LogitechDevice.h

win32:LIBS += $$PWD/win/lglcd.lib
macx:LIBS += $$PWD/mac/liblgLcd.a -framework CoreFoundation

RESOURCES += LgLcdManResources.qrc
