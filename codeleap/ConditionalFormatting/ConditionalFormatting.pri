cf {

QT += xml

DEFINES += LH_CF

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/LH_QtCFInstance.cpp \
    $$PWD/cf_rule.cpp

HEADERS += \
    $$PWD/LH_QtCFInstance.h \
    $$PWD/cf_sources_targets.h \
    $$PWD/cf_rule.h
}
