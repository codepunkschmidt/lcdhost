TARGET = LH_TS3
TEMPLATE = lib
DEFINES += LH_GRAPH_LIBRARY
QT += network

include(../Plugins.pri)

SOURCES += \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtNetwork.cpp \
    LH_QtPlugin_TS3.cpp

HEADERS += \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtNetwork.h \
    ../LH_QtInstance.h \
    LH_QtPlugin_TS3.h \
    LH_TS3_ClientList.h \
    LH_TS3_ChannelList.h

RESOURCES += \
     LH_TS3.qrc
