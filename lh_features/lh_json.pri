LH_FEATURES += lh_json
defineTest(lh_json) {
    LIBS *= -llh_json
    INCLUDEPATH *= $$LH_DIR_INCLUDE/lh_json
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}
