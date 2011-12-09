TARGET = LH_Cursor
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_CURSOR_LIBRARY
CONFIG += cf json

include(../Plugins.pri)

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Decor/LH_Rectangle.h \
    actionType.h \
    LH_QtPlugin_Cursor.h \
    LH_CursorData.h \
    LH_CursorController.h \
    LH_CursorRectangle.h #\
#    LH_CursorPage.h \
#    LH_CursorImage.h \
#    LH_QuickAction.h \
#    LH_CursorAction.h \
#    LH_ArrayTester.h

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Decor/LH_Rectangle.cpp \
    actionType.cpp \
    LH_QtPlugin_Cursor.cpp \
    LH_CursorData.cpp \
    LH_CursorController.cpp \
    LH_CursorRectangle.cpp #\
#    LH_CursorPage.cpp \
#    LH_CursorImage.cpp \
#    LH_QuickAction.cpp \
#    LH_CursorAction.cpp \
#    LH_ArrayTester.cpp
