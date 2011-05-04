TARGET = LH_NowPlaying
TEMPLATE = lib
win32: CONFIG  += qaxcontainer
# qaxserver
QT += network xml
DEFINES += LH_NOWPLAYING_LIBRARY

include(../Plugins.pri)

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

SOURCES += ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_TextNowPlaying.cpp \
    LH_QtPlugin_NowPlaying.cpp \
    ../LH_Text/LH_Text.cpp \
    ../LH_QtInstance.cpp

win32: SOURCES += wmp.c iTunesCOMInterface_i.c

HEADERS += ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_QtPlugin_NowPlaying.h \
    ../LH_Text/LH_Text.h \
    ../LH_QtInstance.h

win32: HEADERS += wmp.h wa_ipc.h iTunesCOMInterface.h
