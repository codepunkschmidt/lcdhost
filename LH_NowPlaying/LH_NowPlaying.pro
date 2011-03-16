TARGET = LH_NowPlaying
TEMPLATE = lib
CONFIG  += qaxserver \
    QAxContainer
QT += network \
    xml
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
    ../LH_QtInstance.cpp \
    wmp.c \
    iTunesCOMInterface_i.c

HEADERS += ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    wmp.h \
    wa_ipc.h \
    iTunesCOMInterface.h \
    LH_QtPlugin_NowPlaying.h \
    ../LH_Text/LH_Text.h \
    ../LH_QtInstance.h
