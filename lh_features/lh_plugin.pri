LH_FEATURES += lh_plugin
defineTest(lh_plugin) {
    contains(TEMPLATE, lib) {
        isEmpty(DESTDIR): error($$_PRO_FILE_: DESTDIR is not set)

        isEmpty(LH_SIGNATURE_PRIVATE_FILE) {
                LH_SIGNATURE_PRIVATE_FILE=$$quote($$LH_DIR_TOP/lcdhost-private.pem)
                !exists($$LH_SIGNATURE_PRIVATE_FILE): LH_SIGNATURE_PRIVATE_FILE=
                else: LH_SIGNATURE_PUBLIC_URL=http://www.linkdata.se/downloads/software/lcdhost/lcdhost-public.pem
        }

        isEmpty(LH_SIGNATURE_PRIVATE_FILE) {
                LH_SIGNATURE_PRIVATE_FILE=$$quote($$LH_DIR_TOP/triscopic-private.pem)
                !exists($$LH_SIGNATURE_PRIVATE_FILE): LH_SIGNATURE_PRIVATE_FILE=
                else: LH_SIGNATURE_PUBLIC_URL=http://www.codeleap.co.uk/lcdhost/public-key
        }

        !isEmpty(LH_SIGNATURE_PUBLIC_URL): exists($$LH_SIGNATURE_PRIVATE_FILE) {
            isEmpty(LH_SIGNPLUGIN) {
                win32: LH_SIGNPLUGIN=$$quote($$LH_DIR_BINARIES/SignPlugin.exe)
                else: LH_SIGNPLUGIN=$$quote($$LH_DIR_BINARIES/SignPlugin)
            }
            QMAKE_POST_LINK=$$quote(\"$$LH_SIGNPLUGIN\" -c -o \
                \"$$LH_SIGNATURE_PRIVATE_FILE\" \"$$LH_SIGNATURE_PUBLIC_URL\" \
                \"$$DESTDIR/$$TARGET\")
            export(LH_SIGNATURE_PRIVATE_FILE)
            export(LH_SIGNATURE_PUBLIC_URL)
            export(QMAKE_POST_LINK)
        }
    }
    return(true)
}
