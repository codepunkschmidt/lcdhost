TARGET = LH_Weather
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_WEATHER_LIBRARY

include(../Plugins.pri)
include(../CF/CF.pri)
include(../GoogleTranslator/GoogleTranslator.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Text/LH_Text.cpp \
    LH_WeatherStructs.cpp \
    LH_QtPlugin_Weather.cpp \
    LH_WeatherText.cpp \
    LH_WeatherImage.cpp \
    LH_WeatherData.cpp \
    LH_WeatherBrowserOpener.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Text/LH_Text.h \
    LH_QtPlugin_Weather.h \
    LH_WeatherText.h \
    LH_WeatherImage.h \
    LH_WeatherData.h \
    LH_WeatherBrowserOpener.h
