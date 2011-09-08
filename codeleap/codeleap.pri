#
# codeleap.pri
#
# Supports the following qmake CONFIG values:
#  json
#  codeleapui
#  conditionalformatting
#  googletranslator
#  
#
# Andy "Triscopic" Bridges is the sole maintainer of this
# file and all the files in this directory and subdirectories. 
# Do *NOT* commit here unless you have his explicit approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "codeleap".
#

LH_CODELEAP_DIR = $$PWD

json {
        LH_JSON_DIR = $$LH_CODELEAP_DIR/json
	INCLUDEPATH += $$LH_JSON_DIR
	HEADERS += $$LH_JSON_DIR/json.h
	SOURCES += $$LH_JSON_DIR/json.cpp
}

codeleapui {
    LH_CODELEAP_UI_DIR = $$LH_CODELEAP_DIR/codeleap_UI

    INCLUDEPATH += $$LH_CODELEAP_UI_DIR

    HEADERS += \
        $$LH_CODELEAP_UI_DIR/LH_Qt_array_int_ui.h \
        $$LH_CODELEAP_UI_DIR/LH_Qt_array_double_ui.h \
        $$LH_CODELEAP_UI_DIR/LH_Qt_array_string_ui.h

    SOURCES += \
        $$LH_CODELEAP_UI_DIR/LH_Qt_array_int_ui.cpp \
        $$LH_CODELEAP_UI_DIR/LH_Qt_array_double_ui.cpp \
        $$LH_CODELEAP_UI_DIR/LH_Qt_array_string_ui.cpp
}

conditionalformatting {
    LH_CF_DIR = $$LH_CODELEAP_DIR/ConditionalFormatting

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
}

googletranslator {
    LH_GOOGLETRANSLATOR_DIR = $$LH_CODELEAP_DIR/GoogleTranslator

    INCLUDEPATH += $$LH_GOOGLETRANSLATOR_DIR

    exists($$LH_GOOGLETRANSLATOR_DIR/TranslationAPIKey.h) {
        DEFINES += GOOGLETRANSLATOR
        SOURCES += $$LH_GOOGLETRANSLATOR_DIR/GoogleTranslator.cpp
        HEADERS += $$LH_GOOGLETRANSLATOR_DIR/TranslationAPIKey.h $$LH_GOOGLETRANSLATOR_DIR/GoogleTranslator.h
    } else {
        error("Missing $$LH_GOOGLETRANSLATOR_DIR/TranslationAPIKey.h")
    }
}
