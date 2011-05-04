TARGET = LH_NowPlaying
TEMPLATE = lib
win32: CONFIG  += qaxserver qaxcontainer
QT += network xml
DEFINES += LH_NOWPLAYING_LIBRARY

include(../Plugins.pri)

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

SOURCES += ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_QtPlugin_NowPlaying.cpp \
    ../LH_Text/LH_Text.cpp \
    ../LH_QtInstance.cpp \
    LH_NP_Winamp.cpp \
    LH_NP_iTunes.cpp \
    utils.cpp \
    disphelper.c \
    LH_NP_MSN_Compat.cpp \
    LH_NowPlayingText.cpp \
    ../LH_Bar/LH_Bar.cpp \
    LH_NowPlayingBar.cpp \
    LH_NowPlayingEmulator.cpp

win32: SOURCES += iTunesCOMInterface_i.c

HEADERS += ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_QtPlugin_NowPlaying.h \
    ../LH_Text/LH_Text.h \
    ../LH_QtInstance.h \
    disphelper.h \
    LH_NowPlayingText.h \
    ../LH_Bar/LH_Bar.h \
    LH_NowPlayingBar.h \
    LH_NowPlayingEmulator.h

win32: HEADERS += wa_ipc.h iTunesCOMInterface.h
