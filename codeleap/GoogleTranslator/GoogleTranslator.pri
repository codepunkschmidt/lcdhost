exists($$PWD/TranslationAPIKey.h) {
    DEFINES += GOOGLETRANSLATOR

    INCLUDEPATH += $$PWD

    SOURCES += $$PWD/GoogleTranslator.cpp

    HEADERS += \
        $$PWD/TranslationAPIKey.h \
        $$PWD/GoogleTranslator.h
} else {
    error("Missing TranslationAPIKey.h")
}
