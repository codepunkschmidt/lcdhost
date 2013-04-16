TEMPLATE = lib
CONFIG += shared lh_shared lh_api5plugin
TARGET = lh_cf
QT += xml
DEFINES += LH_CF

SOURCES += \
    LH_QtCFInstance.cpp \
    cf_rule.cpp

HEADERS += \
    LH_QtCFInstance.h \
    cf_sources_targets.h \
    cf_rule.h

DESTDIR=$$lh_destdir()
