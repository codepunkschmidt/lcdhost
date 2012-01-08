TARGET = LH_TS3
TEMPLATE = lib
DEFINES += LH_TS3_LIBRARY
DEFINES += VERSION=1.01
QT += network
CONFIG += cf

include(../Plugins.pri)

INCLUDEPATH += $$PWD/../LH_Monitoring/LH_QImage

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_QtPlugin_TS3.cpp \
    LH_TS3SpeakingImage.cpp \
    LH_TS3MuteImage.cpp \
    LH_TS3ConnectionImage.cpp \
    LH_TS3SpeakingText.cpp \
    LH_TS3NicknameText.cpp \
    LH_TS3ChannelNameText.cpp \
    LH_TS3CombinedStatusImage.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    ../LH_Text/LH_Text.h \
    LH_QtPlugin_TS3.h \
    LH_TS3_ClientList.h \
    LH_TS3_ChannelList.h \
    LH_TS3MuteImage.h \
    LH_TS3CombinedStatusImage.h

RESOURCES += \
     LH_TS3.qrc
