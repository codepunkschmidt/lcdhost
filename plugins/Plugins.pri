# Provides legacy .pro file compatibility

CONFIG *= lh_plugin lh_api5plugin
INCLUDEPATH *= $$PWD
cf: CONFIG *= lh_cf
datacollection: CONFIG *= lh_datacollection
json: CONFIG *= lh_json
translator: CONFIG *= lh_simpletranslator
# include(../codeleap/codeleap.pri)
DESTDIR=$$lh_destdir()
