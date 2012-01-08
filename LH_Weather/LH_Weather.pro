TARGET = LH_Weather
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_WEATHER_LIBRARY
DEFINES += VERSION=1.51
CONFIG += cf translator json

include(../Plugins.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Text/LH_Text.cpp \
    LH_WeatherData.cpp \
    LH_QtPlugin_Weather.cpp \
    LH_WeatherText.cpp \
    LH_WeatherImage.cpp \
    LH_WeatherBrowserOpener.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Text/LH_Text.h \
    LH_WeatherData.h \
    LH_QtPlugin_Weather.h \
    LH_WeatherText.h \
    LH_WeatherImage.h \
    LH_WeatherBrowserOpener.h

RESOURCES += \
    LH_Weather.qrc
