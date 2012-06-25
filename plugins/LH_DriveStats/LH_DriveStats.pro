TARGET = LH_DriveStats
TEMPLATE = lib
DEFINES += LH_DRIVESTATS_LIBRARY
DEFINES += "COMMON_OBJECT_NAME=\"Drive Performance\""
DEFINES += "MONITORING_FOLDER=\"System/Drives\""
DEFINES += VERSION=1.04

QT += xml
CONFIG += cf

include(../Plugins.pri)

INCLUDEPATH += \
    $$PWD/../LH_Monitoring/LH_QImage \
    $$PWD/../LH_Monitoring \
    $$PWD/../LH_Monitoring/Sources \
    $$PWD/../LH_Monitoring/Objects

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Graph/LH_Graph.h \
    ../LH_Text/LH_Text.h \
    ../LH_Bar/LH_Bar.h \
    ../LH_Dial/LH_Dial.h \
    LH_QtPlugin_DriveStats.h \
    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    ../LH_Monitoring/Objects/LH_MonitoringText.h \
    ../LH_Monitoring/Objects/LH_MonitoringImage.h \
    ../LH_Monitoring/Objects/LH_MonitoringGraph.h \
    ../LH_Monitoring/Objects/LH_MonitoringDial.h \
    ../LH_Monitoring/Objects/LH_MonitoringPie.h \
    ../LH_Monitoring/Objects/LH_MonitoringBar.h \
    ../LH_Monitoring/Sources/LH_MonitoringData.h \
    ../LH_Monitoring/Sources/LH_MonitoringUI.h \
    LH_DriveStatsData.h \
    LH_DriveStatsTypes.h

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Graph/LH_Graph.cpp \
    ../LH_Text/LH_Text.cpp \
    ../LH_Bar/LH_Bar.cpp \
    ../LH_Dial/LH_Dial.cpp \
    LH_QtPlugin_DriveStats.cpp \
    ../LH_Monitoring/LH_QImage/logo_blob.c \
    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    ../LH_Monitoring/Objects/LH_MonitoringText.cpp \
    ../LH_Monitoring/Objects/LH_MonitoringImage.cpp \
    ../LH_Monitoring/Objects/LH_MonitoringGraph.cpp \
    ../LH_Monitoring/Objects/LH_MonitoringDial.cpp \
    ../LH_Monitoring/Objects/LH_MonitoringPie.cpp \
    ../LH_Monitoring/Objects/LH_MonitoringBar.cpp \
    ../LH_Monitoring/Sources/LH_MonitoringData.cpp \
    ../LH_Monitoring/Sources/LH_MonitoringUI.cpp \
    LH_DriveStatsData.cpp










