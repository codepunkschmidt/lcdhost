TARGET = LH_Cursor
TEMPLATE = lib
QT += network \
    xml
DEFINES += LH_CURSOR_LIBRARY

include(../Plugins.pri)

HEADERS += \
    LH_CursorController.h \
    ../LH_QtSetupItem.h \
    ../LH_QtObject.h \
    ../LH_QtPlugin.h \
    LH_QtPlugin_Cursor.h \
    LH_CursorRectangle.h \
    ../LH_Text/LH_Text.h \
    ../LH_QtInstance.h \
    ../LH_Qt_QFileInfo.h \
    LH_CursorData.h \
    LH_CursorPage.h \
    LH_CursorImage.h \
    LH_QuickAction.h \
    LH_CursorAction.h \
    actionType.h

SOURCES += \
    LH_CursorController.cpp \
    ../LH_QtSetupItem.cpp \
    ../LH_QtObject.cpp \
    ../LH_QtPlugin.cpp \
    LH_QtPlugin_Cursor.cpp \
    LH_CursorRectangle.cpp \
    ../LH_Text/LH_Text.cpp \
    ../LH_QtInstance.cpp \
    LH_CursorData.cpp \
    LH_CursorPage.cpp \
    LH_CursorImage.cpp \
    LH_QuickAction.cpp \
    LH_CursorAction.cpp \
    actionType.cpp
