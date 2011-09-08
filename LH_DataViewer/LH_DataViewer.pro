TARGET = LH_DataViewer
TEMPLATE = lib
QT += network xml
DEFINES += LH_DATAVIEWER_LIBRARY

0 { # disabled until Triscopic is ready to attack the A20+ changes

include(../linkdata/LCDHost.pri)
include(../codeleap/ConditionalFormatting.pri)

SOURCES += \
    ../LH_Text/LH_Text.cpp \
    ../LH_Bar/LH_Bar.cpp \
    ../LH_Dial/LH_Dial.cpp \
    LH_QtPlugin_DataViewer.cpp \
    LH_DataViewerConnector.cpp \
    LH_DataViewerText.cpp \
    LH_DataViewerImage.cpp \
    LH_DataViewerBar.cpp \
    LH_DataViewerExpiredImage.cpp \
    LH_DataViewerData.cpp \
    LH_DataViewerDial.cpp
    
HEADERS += \
    ../LH_Text/LH_Text.h \
    ../LH_Bar/LH_Bar.h \
    ../LH_Dial/LH_Dial.h \
    LH_QtPlugin_DataViewer.h \
    LH_DataViewerConnector.h \
    LH_DataViewerText.h \
    LH_DataViewerImage.h \
    LH_DataViewerBar.h \
    LH_DataViewerData.h \
    LH_DataViewerExpiredImage.h \
    LH_DataViewerDataTypes.h \
    LH_DataViewerDial.h
}
