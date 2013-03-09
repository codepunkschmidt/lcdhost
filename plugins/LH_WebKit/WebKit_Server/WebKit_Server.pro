TARGET = WebKitServer
TEMPLATE = app
QT += network webkit
CONFIG -= app_bundle
INCLUDEPATH *= $$dirname(PWD)
LIBS *= -llh_logger

include(../../Plugins.pri)

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
