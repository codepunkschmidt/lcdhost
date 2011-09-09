TARGET = WebKitServer
TEMPLATE = app
QT += webkit network
CONFIG -= app_bundle

load(../../PluginsConfig.prf)

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
