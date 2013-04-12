LH_FEATURES += lh_hid
defineTest(lh_hid) {
    LIBS*=-llh_hid
    export(LIBS)
    INCLUDEPATH*=$$LH_DIR_INCLUDE/lh_hid
    export(INCLUDEPATH)
    return(true)
}
