TRISCOPIC = LH_Cursor LH_Graph LH_Dial LH_Weather LH_TS3 LH_Logic
win32: TRISCOPIC += LH_Monitoring LH_NowPlaying LH_DataViewer LH_DriveStats LH_LCoreReboot

include($$PWD/ConditionalFormatting/ConditionalFormatting.pri)
include($$PWD/GoogleTranslator/GoogleTranslator.pri)
include($$PWD/json/json.pri)
