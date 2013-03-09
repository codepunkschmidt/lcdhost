# (empty)

CONFIG *= lh_plugin
LIBS *= -llh_api5plugin
INCLUDEPATH *= $$dirname(PWD)/linkdata/lh_api5plugin
INCLUDEPATH *= $$PWD
include($$dirname(PWD)/codeleap/codeleap.pri)
DESTDIR=$$lh_destdir()
