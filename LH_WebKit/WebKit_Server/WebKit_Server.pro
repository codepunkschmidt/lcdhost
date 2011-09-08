TARGET = WebKitServer
TEMPLATE = app
QT += webkit network
CONFIG -= app_bundle

include(../../linkdata/linkdata.pri)
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
