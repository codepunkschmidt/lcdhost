TARGET = LH_Monitoring
TEMPLATE = lib
DEFINES += LH_MONITORING_LIBRARY
DEFINES += COMMON_OBJECT_NAME="Monitoring"
DEFINES += "MONITORING_FOLDER=\"3rdParty/System Monitoring\""
DEFINES += DATA_CACHE_MAX_POINTS=2048
DEFINES += VERSION=3.00

QT += xml
CONFIG += cf json datacollection

include(../Plugins.pri)

INCLUDEPATH += $$PWD/LH_QImage
INCLUDEPATH += $$PWD/Sources \
               $$PWD/Objects \
               $$PWD/Types

MONOBJECTS_HEADERS = \
    ../LH_Text/LH_Text.h \
    Objects/LH_MonitoringText.h \
    $$PWD/LH_QImage/LH_QImage.h \
    Objects/LH_MonitoringImage.h \
    ../LH_Graph/LH_Graph.h \
    Objects/LH_MonitoringGraph.h \
    ../LH_Dial/LH_Dial.h \
    Objects/LH_MonitoringDial.h \
    ../LH_Bar/LH_Bar.h \
    Objects/LH_MonitoringBar.h

MONOBJECTS_SOURCES = \
    ../LH_Text/LH_Text.cpp \
    Objects/LH_MonitoringText.cpp \
    $$PWD/LH_QImage/logo_blob.c \
    $$PWD/LH_QImage/LH_QImage.cpp \
    Objects/LH_MonitoringImage.cpp \
    ../LH_Graph/LH_Graph.cpp \
    Objects/LH_MonitoringGraph.cpp \
    ../LH_Dial/LH_Dial.cpp \
    Objects/LH_MonitoringDial.cpp \
    ../LH_Bar/LH_Bar.cpp \
    Objects/LH_MonitoringBar.cpp

MONTYPES_HEADERS = \
    Types/LH_MonitoringTypes_Afterburner.h \
    Types/LH_MonitoringTypes_ATITrayTools.h \
    Types/LH_MonitoringTypes_CoreTemp.h \
    Types/LH_MonitoringTypes_Fraps.h \
    Types/LH_MonitoringTypes_GPUZ.h \
    Types/LH_MonitoringTypes_HWiNFO.h \
    Types/LH_MonitoringTypes_RivaTuner.h \
    Types/LH_MonitoringTypes_SpeedFan.h

MONSOURCES_HEADERS = \
    Sources/LH_MonitoringSource_Afterburner.h \
    Sources/LH_MonitoringSource_Aida64.h \
    Sources/LH_MonitoringSource_ATITrayTools.h \
    Sources/LH_MonitoringSource_CoreTemp.h \
    Sources/LH_MonitoringSource_Fraps.h \
    Sources/LH_MonitoringSource_GPUZ.h \
    Sources/LH_MonitoringSource_HWiNFO.h \
    Sources/LH_MonitoringSource_HWMonitor.h \
    Sources/LH_MonitoringSource_Logitech.h \
    Sources/LH_MonitoringSource_RivaTuner.h \
    Sources/LH_MonitoringSource_SpeedFan.h

MONSOURCES_SOURCES = \
    Sources/LH_MonitoringSource_Afterburner.cpp \
    Sources/LH_MonitoringSource_Aida64.cpp \
    Sources/LH_MonitoringSource_ATITrayTools.cpp \
    Sources/LH_MonitoringSource_CoreTemp.cpp \
    Sources/LH_MonitoringSource_Fraps.cpp \
    Sources/LH_MonitoringSource_GPUZ.cpp \
    Sources/LH_MonitoringSource_HWiNFO.cpp \
    Sources/LH_MonitoringSource_HWMonitor.cpp \
    Sources/LH_MonitoringSource_Logitech.cpp \
    Sources/LH_MonitoringSource_RivaTuner.cpp \
    Sources/LH_MonitoringSource_SpeedFan.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    $$MONTYPES_HEADERS \
    LH_QtPlugin_Monitoring.h \
    Objects/LH_MonitoringObject.h \
    Sources/LH_MonitoringSources.h \
    Sources/LH_MonitoringSource.h \
    $$MONOBJECTS_HEADERS \
    $$MONSOURCES_HEADERS \
    LH_AidaWriter.h \
    LH_RivaWriter.h

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Monitoring.cpp \
    Objects/LH_MonitoringObject.cpp \
    Sources/LH_MonitoringSources.cpp \
    Sources/LH_MonitoringSource.cpp \
    $$MONOBJECTS_SOURCES \
    $$MONSOURCES_SOURCES \
    LH_AidaWriter.cpp \
    LH_RivaWriter.cpp

RESOURCES += \
    LH_Monitoring.qrc
