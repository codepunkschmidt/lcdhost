TARGET = LH_Weather
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_WEATHER_LIBRARY

include(../Plugins.pri)

SOURCES += ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    LH_QtPlugin_Weather.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_WeatherText.cpp \
    LH_WeatherStructs.cpp \
    LH_WeatherImage.cpp \
    LH_WeatherConnector.cpp \
    ../LH_QtInstance.cpp
HEADERS += ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    LH_QtPlugin_Weather.h \
    ../LH_Text/LH_Text.h \
    LH_WeatherText.h \
    LH_WeatherImage.h \
    LH_WeatherConnector.h \
    ../LH_QtInstance.h
