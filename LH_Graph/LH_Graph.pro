TARGET = LH_Graph
TEMPLATE = lib
DEFINES += LH_GRAPH_LIBRARY

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_QtCPU.cpp \
    ../LH_QtNetwork.cpp \
    LH_QtPlugin_Graph.cpp \
    LH_Graph.cpp \
    LH_GraphCPUAverage.cpp \
    LH_GraphCPUHistogram.cpp \
    LH_GraphMemPhysical.cpp \
    LH_GraphMemVirtual.cpp \
    LH_GraphNetIn.cpp \
    LH_GraphNetOut.cpp
HEADERS += \
    $$PLUGIN_SOURCES \
    ../LH_QtCPU.h \
    ../LH_QtNetwork.h \
    LH_QtPlugin_Graph.h \
    LH_Graph.h
