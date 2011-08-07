#include "LH_FrapsData.h"
#include <QDebug>

LH_FrapsData::LH_FrapsData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    Q_UNUSED(includeGroups);

    ui_->reset();
    ui_->setVisible(mon_item);
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Current FPS");
    if(dataMode & mdmText) ui_->append(mon_item, "Game Name");
    //ui_->append(mon_item, "Total Frames");
    //ui_->append(mon_item, "Time of Last Frame");
    ui_->refresh(mon_item);
}

LH_FrapsData::~LH_FrapsData()
{

}


FRAPS_SHARED_DATA *(WINAPI *FrapsSharedData) ();
bool LH_FrapsData::getData(float& value, QString& text, QString& units)
{
#ifdef Q_WS_WIN
    HMODULE frapsDLL;
    FRAPS_SHARED_DATA *fsd;
    frapsDLL = GetModuleHandleA("FRAPS32.DLL");
    value = 0;
    text = "N/A";
    units = "";
    float resultVal = false;

    if (!frapsDLL) {
        //setup_FPS_->setValue("Not running");
        pingFraps();
        resultVal = false;
    } else {
        FrapsSharedData = (FRAPS_SHARED_DATA *(WINAPI *)()) GetProcAddress(frapsDLL, "FrapsSharedData");
        if (!FrapsSharedData){
            //setup_FPS_->setValue("Needs Fraps 1.9C or later!");
            resultVal = false;
        } else {
            if(ui_->value(mon_item) == -1)
            {
                //setup_FPS_->setValue("No option selected");
                resultVal = false;
            } else {
                fsd = FrapsSharedData();
                text = "";
                if(ui_->valueText(mon_item) == "Current FPS")
                {
                    value = fsd->currentFPS;
                    units = "FPS";
                }
                if(ui_->valueText(mon_item) == "Game Name")
                    text =  QString(fsd->gameName);
                if(ui_->valueText(mon_item) == "Total Frames")
                    value =  fsd->totalFrames;
                if(ui_->valueText(mon_item) == "Time of Last Frame")
                    value =  fsd->timeOfLastFrame;
                //setup_FPS_->setValue((text==""? QString::number(value) : text)  + units);
                resultVal = true;
            }
        }
    }
    return resultVal;
#else
    Q_UNUSED(value);
    Q_UNUSED(text);
    Q_UNUSED(units);
    return false;
#endif
}

void LH_FrapsData::pingFraps()
{
#ifdef Q_WS_WIN
    if( FindWindowA( "#32770", NULL ) != (HWND)NULL )
    {
        HWND hWndLH = FindWindowA( "QWidget" , "LCDHost" );
        if ( hWndLH != 0 )
        {
            HINSTANCE instance = (HINSTANCE)GetWindowLong(hWndLH, GWL_HINSTANCE);

            HWND hWnd = CreateWindowA("QWidget", "LCDHost",
                                       WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                       (HWND) NULL,(HMENU) NULL, instance, (LPVOID) NULL);
            DestroyWindow( hWnd );
        }
    }
#else
    return;
#endif
}

monitoringDataType LH_FrapsData::getType()
{
    if( ui_->valueText(mon_item) == "Current FPS" ) return mdtInt;
    if( ui_->valueText(mon_item) == "Game Name" ) return mdtText;
    if( ui_->valueText(mon_item) == "Total Frames" ) return mdtInt;
    if( ui_->valueText(mon_item) == "Time of Last Frame" ) return mdtText; //?
    return mdtInt;
}
