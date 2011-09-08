TARGET = LH_TS3
TEMPLATE = lib
DEFINES += LH_GRAPH_LIBRARY
QT += network

include(../linkdata/LCDHost.pri)
include(../codeleap/ConditionalFormatting.pri)

SOURCES += \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_QtPlugin_TS3.cpp \
    LH_TS3SpeakingImage.cpp \
    LH_TS3MuteImage.cpp \
    LH_TS3ConnectionImage.cpp \
    LH_TS3SpeakingText.cpp \
    LH_TS3NicknameText.cpp \
    LH_TS3ChannelNameText.cpp

HEADERS += \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    ../LH_Text/LH_Text.h \
    LH_QtPlugin_TS3.h \
    LH_TS3_ClientList.h \
    LH_TS3_ChannelList.h \
    LH_TS3MuteImage.h

RESOURCES += \
     LH_TS3.qrc
