TARGET = LH_DataViewer
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_DATAVIEWER_LIBRARY

include(../Plugins.pri)
include(../CF/cf.pri)

SOURCES += ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_QtPlugin_DataViewer.cpp \
    LH_DataViewerConnector.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_DataViewerText.cpp \
    LH_DataViewerImage.cpp \
    LH_DataViewerBar.cpp \
    ../LH_Bar/LH_Bar.cpp \
    LH_DataViewerExpiredImage.cpp \
    ../LH_QtInstance.cpp \
    LH_DataViewerData.cpp \
    LH_DataViewerDial.cpp \
    ../LH_Dial/LH_Dial.cpp
HEADERS += ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_QtPlugin_DataViewer.h \
    LH_DataViewerConnector.h \
    ../LH_Text/LH_Text.h \
    LH_DataViewerText.h \
    LH_DataViewerImage.h \
    LH_DataViewerBar.h \
    ../LH_Bar/LH_Bar.h \
    LH_DataViewerData.h \
    LH_DataViewerExpiredImage.h \
    ../LH_QtInstance.h \
    LH_DataViewerDataTypes.h \
    LH_DataViewerDial.h \
    ../LH_Dial/LH_Dial.h
