TARGET = WebKitServer
TEMPLATE = app
QT += network webkit
greaterThan(QT_MAJOR_VERSION, 4): QT *= webkitwidgets
CONFIG -= app_bundle

include(../../Plugins.pri)
include(../../linkdata/LH_Logger/LH_Logger.pri)
INCLUDEPATH += ..

DESTDIR = $$LCDHOST_BINARIES

SOURCES += \
    ../WebKitCommand.cpp \
    WebKitServerMain.cpp \
    WebKitRequest.cpp \
    WebKitServerWindow.cpp

HEADERS += \
    ../WebKitCommand.h \
    WebKitRequest.h \
    WebKitServerWindow.h \
    EventWebKitHeartbeat.h

FORMS += WebKitServerWindow.ui
