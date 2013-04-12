LH_FEATURES += lh_cf

defineTest(lh_cf) {
    DEFINES *= LH_CF
    LIBS *= -llh_cf
    INCLUDEPATH *= $$LH_DIR_INCLUDE/lh_cf
    export(DEFINES)
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}

# QT += xml
