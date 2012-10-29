TARGET = LH_NowPlaying
TEMPLATE = lib
CONFIG  += QAxContainer
QT += network xml
DEFINES += LH_NOWPLAYING_LIBRARY
DEFINES += VERSION=3.03
DEFINES += USEINTERNET

DEFINES += WINVER=0x0601
DEFINES += _WIN32_WINNT=0x0601

CONFIG += cf

include(../Plugins.pri)
include(../3rdParty/taglib/taglib.pri)

# We don't want warnings from 3rd party C code
QMAKE_CFLAGS_WARN_ON = -w

LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.0/Lib" -lWininet -lshell32

INCLUDEPATH += \
    SDKs \
    SDKs/iTunes \
    Players \
    Helpers

INCLUDEPATH += $$PWD/../LH_Monitoring/LH_QImage

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_NowPlaying.cpp \
    SDKs/iTunes/iTunesCOMInterface_i.c \
    Players/Player.cpp \
    Players/PlayerITunes.cpp \
    Players/PlayerWinamp.cpp \
    Players/PlayerWLM.cpp \
    Players/PlayerFoobar.cpp \
    Players/PlayerVLC.cpp \
    Players/PlayerSpotify.cpp \
    Helpers/Lyrics.cpp \
    Helpers/Internet.cpp \
    Helpers/Cover.cpp \
    ../LH_Text/LH_Text.cpp \
    ../LH_Bar/LH_Bar.cpp \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    Objects/LH_NowPlayingText.cpp \
    Objects/LH_NowPlayingBar.cpp \
    Objects/LH_NowPlayingEmulator.cpp \
    Objects/LH_NowPlayingAlbumArt.cpp \
    Objects/LH_NowPlayingRemoteControl.cpp \
    Objects/LH_NowPlayingImage.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_NowPlaying.h \
    SDKs/iTunes/iTunesCOMInterface.h \
    SDKs/iTunes/DispEx.h \
    Players/Player.h \
    Players/PlayerITunes.h \
    Players/PlayerWinamp.h \
    Players/PlayerWLM.h \
    Players/PlayerFoobar.h \
    Players/PlayerVLC.h \
    Players/PlayerSpotify.h \
    Helpers/Lyrics.h \
    Helpers/Internet.h \
    Helpers/Cover.h \
    ../LH_Text/LH_Text.h \
    ../LH_Bar/LH_Bar.h \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    Objects/LH_NowPlayingText.h \
    Objects/LH_NowPlayingBar.h \
    Objects/LH_NowPlayingEmulator.h \
    Objects/LH_NowPlayingAlbumArt.h \
    Objects/LH_NowPlayingRemoteControl.h \
    Objects/LH_NowPlayingImage.h

RESOURCES += \
    LH_NowPlaying.qrc






























