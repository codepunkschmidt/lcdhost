DEFINES += MONITORING_CLASS
DEFINES += DATA_CACHE_MAX_POINTS=2048

CONFIG += cf json datacollection

INCLUDEPATH += $$PWD/LH_QImage \
               $$PWD/Sources \
               $$PWD/Objects

MONSOURCES_HEADERS = \
    $$PWD/Sources/LH_MonitoringSources.h \
    $$PWD/Sources/LH_MonitoringSource.h

MONSOURCES_SOURCES = \
    $$PWD/Sources/LH_MonitoringSources.cpp \
    $$PWD/Sources/LH_MonitoringSource.cpp

MONOBJECTS_HEADERS = \
    $$PWD/LH_QtMonitoringPlugin.h \
    $$PWD/Objects/LH_MonitoringObject.h \
    $$PWD/../LH_Text/LH_Text.h \
    $$PWD/Objects/LH_MonitoringText.h \
    $$PWD/LH_QImage/LH_QImage.h \
    $$PWD/Objects/LH_MonitoringImage.h \
    $$PWD/../LH_Graph/LH_Graph.h \
    $$PWD/Objects/LH_MonitoringGraph.h \
    $$PWD/../LH_Dial/LH_Dial.h \
    $$PWD/Objects/LH_MonitoringDial.h \
    #$$PWD/Objects/LH_MonitoringPie.h \
    $$PWD/../LH_Bar/LH_Bar.h \
    $$PWD/Objects/LH_MonitoringBar.h \
    $$PWD/../LH_Decor/LH_Rectangle.h \
    $$PWD/../LH_ColorSwatch/LH_ColorSwatch.h \
    $$PWD/Objects/LH_MonitoringColorSwatch.h

MONOBJECTS_SOURCES = \
    $$PWD/LH_QtMonitoringPlugin.cpp \
    $$PWD/Objects/LH_MonitoringObject.cpp \
    $$PWD/../LH_Text/LH_Text.cpp \
    $$PWD/Objects/LH_MonitoringText.cpp \
    $$PWD/LH_QImage/logo_blob.c \
    $$PWD/LH_QImage/LH_QImage.cpp \
    $$PWD/Objects/LH_MonitoringImage.cpp \
    $$PWD/../LH_Graph/LH_Graph.cpp \
    $$PWD/Objects/LH_MonitoringGraph.cpp \
    $$PWD/../LH_Dial/LH_Dial.cpp \
    $$PWD/Objects/LH_MonitoringDial.cpp \
    #$$PWD/Objects/LH_MonitoringPie.cpp \
    $$PWD/../LH_Bar/LH_Bar.cpp \
    $$PWD/Objects/LH_MonitoringBar.cpp \
    $$PWD/../LH_Decor/LH_Rectangle.cpp \
    $$PWD/../LH_ColorSwatch/LH_ColorSwatch.cpp \
    $$PWD/Objects/LH_MonitoringColorSwatch.cpp

