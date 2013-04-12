LH_FEATURES += lh_datacollection

defineTest(lh_datacollection) {
    LIBS *= -llh_datacollection
    INCLUDEPATH *= $$LH_DIR_INCLUDE/lh_datacollection
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}
