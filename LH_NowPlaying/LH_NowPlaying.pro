TARGET = LH_NowPlaying
TEMPLATE = lib
CONFIG  += qaxcontainer
QT += network xml
DEFINES += LH_NOWPLAYING_LIBRARY

include(../Plugins.pri)
include(../CF/cf.pri)

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

SOURCES += ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_Text/LH_Text.cpp \
    ../LH_QtInstance.cpp \
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
    LH_NowPlayingAlbumArt.cpp \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    LH_NowPlayingStatusImage.cpp \
    LH_NowPlayingReader.cpp

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
    wa_ipc.h \
    iTunesCOMInterface.h \
    LH_NowPlayingEmulator.h \
    utils.h \
    LH_NowPlayingAlbumArt.h \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    LH_NowPlayingStatusImage.h \
    LH_NowPlayingReader.h
