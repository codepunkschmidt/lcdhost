TARGET = LH_LgLcdMan
TEMPLATE = lib
DEFINES += LGLCDMAN_LIBRARY
CONFIG += device wow64

include(../Plugins.pri)

win32 {
    SOURCES += \
        $$PLUGIN_SOURCES \
        LH_LgLcdMan.cpp \
        LogitechDevice.cpp \
        LH_LgLcdLegacyThread.cpp \
        LH_LgLcdCallbackThread.cpp \
        LH_LgLcdThread.cpp

    HEADERS += \
        $$PLUGIN_HEADERS \
        LH_LgLcdMan.h \
        LogitechDevice.h \
        EventLgLcdNotification.h \
        LH_LgLcdLegacyThread.h \
        EventLgLcdButton.h \
        LH_LgLcdCallbackThread.h \
        LH_LgLcdThread.h

    LIBS += $$PWD/win/lglcd.lib
    RESOURCES += LgLcdManResources.qrc
}
