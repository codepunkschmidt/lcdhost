TARGET = LH_TorrentMon
TEMPLATE = lib
DEFINES += LH_TORRENTMON_LIBRARY
QT += xml \
      xmlpatterns \
      network

include(../Plugins.pri)
#include(../cf/cf.pri)

HEADERS += $$PLUGIN_HEADERS \
    LH_QtPlugin_TorrentMon.h \
#    ../LH_Graph/LH_Graph.h \
#    ../LH_Text/LH_Text.h \
#    ../LH_Bar/LH_Bar.h \
#    ../LH_Dial/LH_Dial.h \
    ../json.h

SOURCES += $$PLUGIN_SOURCES \
    LH_QtPlugin_TorrentMon.cpp \
#    ../LH_Graph/LH_Graph.cpp \
#    ../LH_Text/LH_Text.cpp \
#    ../LH_Bar/LH_Bar.cpp \
#     ../LH_Dial/LH_Dial.cpp \
    ../json.cpp

RESOURCES +=
