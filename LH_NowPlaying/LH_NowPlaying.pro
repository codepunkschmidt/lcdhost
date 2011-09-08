TARGET = LH_NowPlaying
TEMPLATE = lib
QT += network xml
DEFINES += LH_NOWPLAYING_LIBRARY

include(../linkdata/LCDHost.pri)
include(../codeleap/ConditionalFormatting.pri)

win32:CONFIG  += qaxcontainer

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

win32:SOURCES += \
    ../LH_Text/LH_Text.cpp \
    ../LH_Bar/LH_Bar.cpp \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
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
    LH_NowPlayingStatusImage.cpp \
    LH_NowPlayingReader.cpp \
    LH_NowPlayingThread.cpp

win32:HEADERS += \
    ../LH_Text/LH_Text.h \
    ../LH_Bar/LH_Bar.h \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    LH_QtPlugin_NowPlaying.h \
    disphelper.h \
    LH_NowPlayingText.h \
    LH_NowPlayingBar.h \
    wa_ipc.h \
    iTunesCOMInterface.h \
    LH_NowPlayingEmulator.h \
    utils.h \
    LH_NowPlayingAlbumArt.h \
    LH_NowPlayingStatusImage.h \
    LH_NowPlayingReader.h \
    LH_NowPlayingThread.h
