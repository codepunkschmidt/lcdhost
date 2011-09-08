TARGET = LH_Mailcount
TEMPLATE = lib
DEFINES += LH_MAILCOUNT_LIBRARY

include(../linkdata/LCDHost.pri)
include(../codeleap/ConditionalFormatting.pri)

SOURCES += \
    ../LH_Text/LH_Text.cpp \
    LH_Mailcount.cpp \
    LH_QtPlugin_Mailcount.cpp \
    LH_MailcountText.cpp

HEADERS += \
    ../LH_Text/LH_Text.h \
    LH_Mailcount.h \
    LH_QtPlugin_Mailcount.h \
    LH_MailcountText.h

RESOURCES += LH_Mailcount.qrc
