LH_FEATURES += lh_libusb
defineTest(lh_libusb) {
    LIBS *= -llh_libusb
    win32: LIBS *= -luser32
    INCLUDEPATH *= $$LH_DIR_INCLUDE/lh_libusb
    export(LIBS)
    export(INCLUDEPATH)
    return(true)
}
