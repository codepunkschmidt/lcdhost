LH_FEATURES += lh_zlib
defineTest(lh_zlib) {
    LIBS *= -llh_zlib
    INCLUDEPATH *= $$LH_DIR_INCLUDE/lh_zlib
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}
