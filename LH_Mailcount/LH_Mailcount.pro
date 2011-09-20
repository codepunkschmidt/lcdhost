TARGET = LH_Mailcount
TEMPLATE = lib
DEFINES += LH_MAILCOUNT_LIBRARY

include(../Plugins.pri)
include(../CF/CF.pri)

SOURCES += \
    $$PLUGIN_SOURCES \
    ../LH_Text/LH_Text.cpp \
    LH_Mailcount.cpp \
    LH_QtPlugin_Mailcount.cpp \
    LH_MailcountText.cpp

HEADERS += \
    $$PLUGIN_HEADERS \
    ../LH_Text/LH_Text.h \
    LH_Mailcount.h \
    LH_QtPlugin_Mailcount.h \
    LH_MailcountText.h

RESOURCES += LH_Mailcount.qrc
