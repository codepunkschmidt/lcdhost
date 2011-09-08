#
# GoogleTranslator.pri
#
# Andy "Triscopic" Bridges is the sole maintainer of this
# file and all the files in this directory and subdirectories. 
# Do *NOT* commit here unless you have his explicit approval to do so.
#
# If you find a bug or need a feature or change, submit an issue
# at "http://code.google.com/p/lcdhost/issues/list" and label it
# "codeleap-googletranslator".
#

LH_GOOGLETRANSLATOR_DIR = $$PWD/GoogleTranslator

INCLUDEPATH += $$LH_GOOGLETRANSLATOR_DIR

exists($$LH_GOOGLETRANSLATOR_DIR/TranslationAPIKey.h) {
    DEFINES += GOOGLETRANSLATOR
    SOURCES += $$LH_GOOGLETRANSLATOR_DIR/GoogleTranslator.cpp
    HEADERS += $$LH_GOOGLETRANSLATOR_DIR/TranslationAPIKey.h $$LH_GOOGLETRANSLATOR_DIR/GoogleTranslator.h
} else {
    error("Missing $$LH_GOOGLETRANSLATOR_DIR/TranslationAPIKey.h")
}

