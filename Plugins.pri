#
# LCDHost API5 compatiblity layer
#

CONFIG(debug, debug|release):RELDEB = Debug
else:RELDEB = Release

CHANGESET_REVISION = $$system(hg log -l 1 --template {node}/{rev} $$PWD/$$TARGET)
CHANGESET = $$section(CHANGESET_REVISION,"/",0,0)
REVISION = $$section(CHANGESET_REVISION,"/",1,1)
isEmpty( REVISION ): REVISION = 0
DEFINES += CHANGESET=\"\\\"$$CHANGESET\\\"\"
DEFINES += REVISION=$$REVISION

win32 {
    LCDHOST_PLUGINS = $$PWD/../$$RELDEB/plugins
    LCDHOST_BINARIES = $$PWD/../$$RELDEB
}

macx {
    LCDHOST_PLUGINS = $$PWD/../$$RELDEB/LCDHost.app/Contents/PlugIns
    LCDHOST_BINARIES = $$PWD/../$$RELDEB/LCDHost.app/Contents/MacOS
}

unix:!macx {
    LCDHOST_PLUGINS = $$PWD/../$$RELDEB/plugins
    LCDHOST_BINARIES = $$PWD/../$$RELDEB
}

CONFIG += lh_plugin
hid: CONFIG += lh_hidapi
usb: CONFIG += lh_libusb

INCLUDEPATH += $$PWD

cf: include($$PWD/codeleap/ConditionalFormatting/ConditionalFormatting.pri)
translator: include($$PWD/codeleap/GoogleTranslator/GoogleTranslator.pri)

load($$PWD/PluginsConfig.prf)

contains( TEMPLATE, lib ) {
    exists($$PWD/../lcdhost-private.pem) {
        QMAKE_POST_LINK = \
            $$LCDHOST_BINARIES/SignPlugin -c -o \
            $$PWD/../lcdhost-private.pem \
            http://www.linkdata.se/downloads/software/lcdhost/lcdhost-public.pem \
            $$DESTDIR/$$TARGET
    }
    exists($$PWD/../sign.local) {
        QMAKE_POST_LINK = \
            $$LCDHOST_BINARIES/SignPlugin -c -o \
            $$PWD/../triscopic-private.pem \
            file:///$$PWD/../triscopic-public.pem \
            $$DESTDIR/$$TARGET
    }
    !exists($$PWD/../sign.local) {
        exists($$PWD/../triscopic-private.pem) {
            QMAKE_POST_LINK = \
                $$LCDHOST_BINARIES/SignPlugin -c -o \
                $$PWD/../triscopic-private.pem \
                http://www.codeleap.co.uk/lcdhost/public-key \
                $$DESTDIR/$$TARGET
        }
    }
}
