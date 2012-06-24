TARGET = LH_Graph
TEMPLATE = lib
DEFINES += LH_GRAPH_LIBRARY
DEFINES += VERSION=2.00

CONFIG += systemstats datacollection

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Graph.cpp \
    LH_Graph.cpp \
    LH_GraphCPUAverage.cpp \
    LH_GraphCPUHistogram.cpp \
    LH_GraphMemPhysical.cpp \
    LH_GraphMemVirtual.cpp \
    LH_GraphNetIn.cpp \
    LH_GraphNetOut.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_Graph.h \
    LH_Graph.h
