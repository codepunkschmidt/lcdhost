LH_FEATURES += lh_api5plugin
defineTest(lh_api5plugin) {
    LIBS *= -llh_api5plugin
    export(LIBS)
    INCLUDEPATH *= $$LH_DIR_INCLUDE/lh_api5plugin
    export(INCLUDEPATH)
    return(true)
}
