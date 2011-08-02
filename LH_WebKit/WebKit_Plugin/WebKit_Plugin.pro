TARGET = LH_WebKit
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_WEBKIT_LIBRARY

include(../../Plugins.pri)
include(../../CF/CF.pri)

SOURCES += \
    ../../lh_plugin.c \
    ../../LH_QtPlugin.cpp \
    ../../LH_QtInstance.cpp \
    ../../LH_QtObject.cpp \
    ../../LH_QtSetupItem.cpp \
    ../WebKitCommand.cpp \
    ../../LH_Text/LH_Text.cpp \
    LH_QtPlugin_WebKit.cpp \
    LH_WebKit.cpp \
    LH_WebKitHTML.cpp \
    LH_WebKitURL.cpp \
    LH_WebKitFile.cpp \
    LH_RSSText.cpp \
    LH_RSSFeed.cpp \
    LH_RSSInterface.cpp \
    LH_RSSBody.cpp

HEADERS += \
    ../../lh_plugin.h \
    ../../LH_QtPlugin.h \
    ../../LH_QtInstance.h \
    ../../LH_QtObject.h \
    ../../LH_QtSetupItem.h \
    ../WebKitCommand.h \
    ../../LH_Text/LH_Text.h \
    LH_QtPlugin_WebKit.h \
    LH_WebKit.h \
    LH_WebKitHTML.h \
    LH_WebKitURL.h \
    LH_WebKitFile.h \
    LH_RSSText.h \
    LH_RSSFeed.h \
    LH_RSSInterface.h \
    LH_RSSBody.h
