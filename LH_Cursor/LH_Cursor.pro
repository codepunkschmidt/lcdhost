TARGET = LH_Cursor
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_CURSOR_LIBRARY

include(../Plugins.pri)
include(../CF/CF.pri)

HEADERS += \
    $$PLUGIN_HEADERS \
    LH_CursorController.h \
    LH_QtPlugin_Cursor.h \
    LH_CursorRectangle.h \
    LH_CursorData.h \
    LH_CursorPage.h \
    LH_CursorImage.h \
    LH_QuickAction.h \
    LH_CursorAction.h \
    actionType.h

SOURCES += \
    $$PLUGIN_SOURCES \
    LH_CursorController.cpp \
    LH_QtPlugin_Cursor.cpp \
    LH_CursorRectangle.cpp \
    LH_CursorData.cpp \
    LH_CursorPage.cpp \
    LH_CursorImage.cpp \
    LH_QuickAction.cpp \
    LH_CursorAction.cpp \
    actionType.cpp
