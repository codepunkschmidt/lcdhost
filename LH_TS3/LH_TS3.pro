TARGET = LH_TS3
TEMPLATE = lib
DEFINES += LH_GRAPH_LIBRARY
QT += network

include(../Plugins.pri)
include(../CF/cf.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtNetwork.cpp \
    LH_QtPlugin_TS3.cpp \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_TS3MuteImage.cpp

HEADERS += \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtNetwork.h \
    ../LH_QtInstance.h \
    LH_QtPlugin_TS3.h \
    LH_TS3_ClientList.h \
    LH_TS3_ChannelList.h \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    ../LH_Text/LH_Text.h \
    LH_TS3MuteImage.h

RESOURCES += \
     LH_TS3.qrc
