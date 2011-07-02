TARGET = LH_Weather
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_WEATHER_LIBRARY

include(../Plugins.pri)


exists(../TranslationAPIKey.h) {
    DEFINES += GOOGLETRANSLATOR
    SOURCES += ../GoogleTranslator.cpp
    HEADERS += ../TranslationAPIKey.h ../GoogleTranslator.h
} else {
    error("Missing TranslationAPIKey.h")
}

SOURCES += ../lh_plugin.c \
    ../LH_QtPlugin.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtInstance.cpp \
    ../LH_Text/LH_Text.cpp \
    LH_WeatherStructs.cpp \
    LH_QtPlugin_Weather.cpp \
    LH_WeatherText.cpp \
    LH_WeatherImage.cpp \
    LH_WeatherData.cpp \
    LH_WeatherBrowserOpener.cpp
HEADERS += ../lh_plugin.h \
    ../LH_QtPlugin.h \
    ../LH_QtObject.h \
    ../LH_QtSetupItem.h \
    ../LH_QtInstance.h \
    ../LH_Text/LH_Text.h \
    LH_QtPlugin_Weather.h \
    LH_WeatherText.h \
    LH_WeatherImage.h \
    LH_WeatherData.h \
    LH_WeatherBrowserOpener.h
