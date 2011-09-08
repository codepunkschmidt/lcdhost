#
# ConditionalFormatting.pri
#
# Andy "Triscopic" Bridges is the sole maintainer of this
# file and all the files in this directory and subdirectories. 
# Do *NOT* commit here unless you have his explicit approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "codeleap-cf".
#

LH_CF_DIR = $$PWD/ConditionalFormatting

DEFINES += LH_CF

QT += xml

INCLUDEPATH += $$LH_CF_DIR

SOURCES += \
    $$LH_CF_DIR/LH_QtCFInstance.cpp \
    $$LH_CF_DIR/cf_rule.cpp

HEADERS += \
    $$LH_CF_DIR/LH_QtCFInstance.h \
    $$LH_CF_DIR/cf_sources_targets.h \
    $$LH_CF_DIR/cf_rule.h

RESOURCES += \
    $$LH_CF_DIR/cf.qrc
