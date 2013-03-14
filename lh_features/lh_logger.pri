LH_FEATURES += lh_logger
defineTest(lh_logger) {
    LIBS*=-llh_logger
    INCLUDEPATH*=$$LH_DIR_INCLUDE/lh_logger
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}
