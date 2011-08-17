TARGET = LH_Cursor
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_CURSOR_LIBRARY

include(../Plugins.pri)
include(../cf/cf.pri)

HEADERS += $$PLUGIN_HEADERS \
    ../LH_Qt_QFileInfo.h \
#    ../LH_Monitoring/LH_QImage/LH_QImage.h \
    ../LH_Decor/LH_Rectangle.h \
    ../json.h \
    LH_QtPlugin_Cursor.h \
    LH_CursorData.h \
    LH_CursorController.h \
    LH_CursorRectangle.h \
#    LH_CursorPage.h \
#    LH_CursorImage.h \
#    LH_QuickAction.h \
#    LH_CursorAction.h \
    actionType.h

SOURCES += $$PLUGIN_SOURCES \
#    ../LH_Monitoring/LH_QImage/logo_blob.c \
#    ../LH_Monitoring/LH_QImage/LH_QImage.cpp \
    ../LH_Decor/LH_Rectangle.cpp \
    ../json.cpp \
    LH_QtPlugin_Cursor.cpp \
    LH_CursorData.cpp \
    LH_CursorController.cpp \
    LH_CursorRectangle.cpp \
#    LH_CursorPage.cpp \
#    LH_CursorImage.cpp \
#    LH_QuickAction.cpp \
#    LH_CursorAction.cpp \
    actionType.cpp
