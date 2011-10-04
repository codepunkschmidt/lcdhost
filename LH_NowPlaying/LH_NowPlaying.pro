TARGET = LH_NowPlaying
TEMPLATE = lib
CONFIG  += qaxcontainer
QT += network xml
DEFINES += LH_NOWPLAYING_LIBRARY
CONFIG += cf

include(../Plugins.pri)

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Text/LH_Text.cpp \
    ../LH_Bar/LH_Bar.cpp \
    LH_QtPlugin_NowPlaying.cpp \
    LH_NP_iTunes.cpp \
    LH_NP_Winamp.cpp  \
    LH_NP_MSN_Compat.cpp \
    LH_NowPlayingText.cpp \
    LH_NowPlayingBar.cpp \
    LH_NowPlayingEmulator.cpp \
    iTunesCOMInterface_i.c  \
    utils.cpp \
    disphelper.c \
    LH_NowPlayingAlbumArt.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Text/LH_Text.h \
    ../LH_Bar/LH_Bar.h \
    LH_QtPlugin_NowPlaying.h \
    disphelper.h \
    LH_NowPlayingText.h \
    LH_NowPlayingBar.h \
    wa_ipc.h \
    iTunesCOMInterface.h \
    LH_NowPlayingEmulator.h \
    utils.h \
    LH_NowPlayingAlbumArt.h
