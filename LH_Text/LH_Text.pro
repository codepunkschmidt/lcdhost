TARGET = LH_Text
TEMPLATE = lib
QT += network xml
DEFINES += LH_TEXT_LIBRARY

include(../Plugins.pri)

SOURCES += \
    ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtInstance.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_QtPlugin_Text.cpp \
    LH_Text.cpp \
    LH_TextTime.cpp \
    LH_TextDate.cpp \
    LH_TextCPULoad.cpp \
    LH_TextRSS.cpp \
    LH_TextFile.cpp \
    LH_TextNumber.cpp \
    LH_TextNetInbound.cpp \
    LH_TextNetOutbound.cpp \
    LH_TextMemPhysicalFree.cpp \
    LH_TextMemPhysicalTotal.cpp \
    LH_TextMemPhysicalUsed.cpp \
    LH_TextMemVirtualFree.cpp \
    LH_TextMemVirtualTotal.cpp \
    LH_TextMemVirtualUsed.cpp \
    LH_TextStatic.cpp

HEADERS += \
    ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtInstance.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_QtPlugin_Text.h \
    LH_Text.h \
    LH_TextRSS.h \
    LH_TextFile.h \
    LH_TextNumber.h
