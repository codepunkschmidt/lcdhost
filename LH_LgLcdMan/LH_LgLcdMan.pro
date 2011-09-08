TARGET = LH_LgLcdMan
TEMPLATE = lib
DEFINES += LGLCDMAN_LIBRARY

win32 {
    include(../linkdata/linkdata.pri)

    SOURCES += \
        wow64.cpp \
        LH_LgLcdMan.cpp \
        LH_LgLcdLegacyThread.cpp \
        LH_LgLcdCallbackThread.cpp \
        LH_LgLcdThread.cpp \
        LogitechLegacyManager.cpp \
        LogitechManager.cpp \
        LogitechLegacyDevice.cpp \
        LogitechDevice.cpp \
        LogitechCallbackDevice.cpp \
        LogitechCallbackManager.cpp \
        LogitechInputDevice.cpp

    HEADERS += \
        wow64.h \
        LH_LgLcdMan.h \
        EventLgLcdNotification.h \
        LH_LgLcdLegacyThread.h \
        EventLgLcdButton.h \
        LH_LgLcdCallbackThread.h \
        LH_LgLcdThread.h \
        LogitechLegacyManager.h \
        LogitechManager.h \
        LogitechLegacyDevice.h \
        LogitechDevice.h \
        LogitechCallbackDevice.h \
        LogitechCallbackManager.h \
        LogitechInputDevice.h

    RESOURCES += LgLcdManResources.qrc
    LIBS += $$PWD/win/lglcd.lib
}
