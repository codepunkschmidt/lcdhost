LH_FEATURES += lh_shared
defineTest(lh_shared) {
    contains(TEMPLATE, lib): !contains(CONFIG, staticlib) {
        !isEmpty(LH_SHARED_HEADERS) {
            hdr_destdir=$$lh_systempath($${LH_DIR_INCLUDE}/$${TARGET})
            lh_mkdir($$hdr_destdir)
            for(hdr_file, LH_SHARED_HEADERS) {
                hdr_file=$$lh_systempath($${_PRO_FILE_PWD_}/$${hdr_file})
                exists($$hdr_file): lh_copy($$hdr_file, $$hdr_destdir)
            }
        }

        disabled_: win32 {
            lib_srcfiles=$$lh_systempath($${LH_DIR_BINARIES}/$${TARGET})
            lib_destdir=$$lh_systempath($${LH_DIR_LIB})
            lh_mkdir($${lib_destdir})
            QMAKE_POST_LINK+=$$quote($${QMAKE_COPY} \"$${lib_srcfiles}\"*.lib \"$${lib_destdir}\")
            export(QMAKE_POST_LINK)
        }
    }
    return(true)
}
