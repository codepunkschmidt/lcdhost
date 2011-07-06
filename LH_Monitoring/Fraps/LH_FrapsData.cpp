#include "LH_FrapsData.h"
#include <QDebug>

LH_FrapsData::LH_FrapsData(LH_QtObject *parent, monitoringDataMode dataMode) : LH_MonitoringData( parent )
{
    QStringList valueTypes;
    if(dataMode & mdmNumbers) valueTypes.append("Current FPS");
    if(dataMode & mdmText) valueTypes.append("Game Name");
    //valueTypes.append("Total Frames");
    //valueTypes.append("Time of Last Frame");

    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Item", valueTypes);
    setup_value_type_->setHelp( "<p>The value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Item)",0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_type_index_->setOrder(-4);

    if (valueTypes.count()==1) {
        setup_value_type_->setValue(0);
        setup_value_type_->setFlag(LH_FLAG_HIDDEN, true);
    }

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_type_index_, SIGNAL(changed()), parent, SLOT(setTypeSelection()) );
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
            if(setup_value_type_->value() == -1)
            {
                //setup_FPS_->setValue("No option selected");
                resultVal = false;
            } else {
                fsd = FrapsSharedData();
                text = "";
                if(setup_value_type_->list().at(setup_value_type_->value()) == "Current FPS")
                {
                    value = fsd->currentFPS;
                    units = "FPS";
                }
                if(setup_value_type_->list().at(setup_value_type_->value()) == "Game Name")
                    text =  QString(fsd->gameName);
                if(setup_value_type_->list().at(setup_value_type_->value()) == "Total Frames")
                    value =  fsd->totalFrames;
                if(setup_value_type_->list().at(setup_value_type_->value()) == "Time of Last Frame")
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
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Current FPS")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Game Name")
        return mdtText;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Total Frames")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Time of Last Frame")
        return mdtText; //?

    return mdtInt;
}

void LH_FrapsData::setTypeSelection()
{
    qDebug()<<"fraps type selected";
    if (setup_value_type_->list().length()!=0)
        setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_FrapsData::changeTypeSelection()
{
    qDebug()<<"fraps type changed";
    if (setup_value_type_->list().length()!=0)
        setup_value_type_index_->setValue(setup_value_type_->value());
}
