TARGET = LH_Logic
TEMPLATE = lib
# CONFIG  += qaxcontainer
QT += network xml
DEFINES += LH_LOGIC_LIBRARY
DEFINES += VERSION=1.00

CONFIG += cf

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_QtPlugin_Logic.cpp \
    LH_LogicBox.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_QtPlugin_Logic.h \ 
    LH_LogicBox.h



