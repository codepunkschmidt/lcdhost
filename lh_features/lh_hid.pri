LH_FEATURES += lh_hid
defineTest(lh_hid) {
    LIBS*=-llh_hid
    INCLUDEPATH*=$$LH_DIR_INCLUDE/lh_hid
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}
