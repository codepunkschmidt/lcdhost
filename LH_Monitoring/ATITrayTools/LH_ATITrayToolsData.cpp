#include "LH_ATITrayToolsData.h"

LH_ATITrayToolsData::LH_ATITrayToolsData(LH_QtObject *parent, monitoringDataMode dataMode) : LH_MonitoringData( parent )
{
    valueTypes = QStringList();
    //Hardware monitoring
    if(dataMode & mdmNumbers) valueTypes.append("GPU Speed");
    if(dataMode & mdmNumbers) valueTypes.append("MEM Speed");
    if(dataMode & mdmText) valueTypes.append("Game Profile Active");        // 1 = Yes; 0 = No
    if(dataMode & mdmText) valueTypes.append("3D Active");                  // 1 = 3D;  0 = 2D
    if(dataMode & mdmText) valueTypes.append("Temp Mon Supported");         // 1 = Yes;  0 = No
    if(dataMode & mdmNumbers) valueTypes.append("GPU Temp");
    if(dataMode & mdmNumbers) valueTypes.append("ENV Temp");
    if(dataMode & mdmNumbers) valueTypes.append("FAN Duty");
    if(dataMode & mdmNumbers) valueTypes.append("MAX GPU Temp");
    if(dataMode & mdmNumbers) valueTypes.append("MIN GPU Temp");
    if(dataMode & mdmNumbers) valueTypes.append("MAX ENV Temp");
    if(dataMode & mdmNumbers) valueTypes.append("MIN ENV Temp");
    if(dataMode & mdmNumbers) valueTypes.append("Direct3D Antialiasing");   // What values can this have???
    if(dataMode & mdmNumbers) valueTypes.append("Direct3D Anisotropy");     // What values can this have???
    if(dataMode & mdmNumbers) valueTypes.append("OpenGL Antialiasing");     // What values can this have???
    if(dataMode & mdmNumbers) valueTypes.append("OpenGL Anisotropy");       // What values can this have???
    if(dataMode & mdmText) valueTypes.append("3D Active");                  // 1 = Yes;  0 = No (I think)
    if(dataMode & mdmNumbers) valueTypes.append("FPS");
    if(dataMode & mdmNumbers) valueTypes.append("Free Video Memory");
    if(dataMode & mdmNumbers) valueTypes.append("Free Texture Memory");
    if(dataMode & mdmText) valueTypes.append("Current Graphics API");       // 0 - OpenGL, 1 - Direct3D8, 2 - Direct3D9, 3 - DirectDraw, 4 - Direct3Dx (old direct3d api), 5 - Video Overlay, -1 - No active rendering application
    if(dataMode & mdmNumbers) valueTypes.append("Memory Used");

    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Item", valueTypes, LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Item)",0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_type_index_->setOrder(-4);

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_type_index_, SIGNAL(changed()), parent, SLOT(setTypeSelection()) );
}

LH_ATITrayToolsData::~LH_ATITrayToolsData()
{

}

bool LH_ATITrayToolsData::getData(float& value, QString& text, QString& units)
{
#ifdef Q_WS_WIN
    const char* mapname  = "ATITRAY_SMEM";
    float resultVal = true;

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ,true,mapname);
    // Get pointer
    if(filemap != NULL)
    {
        tagTATTData* memoryData = (tagTATTData*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(tagTATTData));

        if (memoryData) {
            getSelectedValue(memoryData, value, text, units);
            UnmapViewOfFile(memoryData);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    return resultVal;
#else
    Q_UNUSED(value);
    Q_UNUSED(text);
    Q_UNUSED(units);
    return false;
#endif
}

void LH_ATITrayToolsData::getSelectedValue(tagTATTData* memoryData, float& value, QString& text, QString& units)
{
    QString type = valueTypes.at(setup_value_type_->value());

    units = "";
    text = "";
    value = 0;


    if(type=="GPU Speed")
    {
        value = memoryData->CurGPU;
        units = "MHz";
    }
    if(type=="MEM Speed")
    {
        value = memoryData->CurMEM;
        units = "MHz";
    }
    if(type=="Game Profile Active")
        text = (memoryData->isGameActive==1? "Yes" : "No");
    if(type=="3D Active")
        text = (memoryData->is3DActive==1? "3D" : "2D");
    if(type=="Temp Mon Supported")
        text = (memoryData->isTempMonSupported==1? "Yes" : "No");
    if(type=="GPU Temp")
    {
        value = memoryData->GPUTemp;
        units = "°C";
    }
    if(type=="ENV Temp")
    {
        value = memoryData->ENVTemp;
        units = "°C";
    }
    if(type=="FAN Duty")
    {
        value = memoryData->FanDuty;
        units = "%";
    }
    if(type=="MAX GPU Temp")
    {
        value = memoryData->MAXGpuTemp;
        units = "°C";
    }
    if(type=="MIN GPU Temp")
    {
        value = memoryData->MINGpuTemp;
        units = "°C";
    }
    if(type=="MAX ENV Temp")
    {
        value = memoryData->MAXEnvTemp;
        units = "°C";
    }
    if(type=="MIN ENV Temp")
    {
        value = memoryData->MINEnvTemp;
        units = "°C";
    }
    if(type=="Direct3D Antialiasing")
        value = memoryData->CurD3DAA;
    if(type=="Direct3D Anisotropy")
        value = memoryData->CurD3DAF;
    if(type=="OpenGL Antialiasing")
        value = memoryData->CurOGLAA;
    if(type=="OpenGL Anisotropy")
        value = memoryData->CurOGLAF;
    if(type=="3D Active")
        text = (memoryData->isTempMonSupported==1? "Yes" : "No");
    if(type=="FPS")
    {
        value = memoryData->CurFPS;
        units = "FPS";
    }
    if(type=="Free Video Memory")
    {
        value = memoryData->FreeVideo;
        units = "MB";
    }
    if(type=="Free Texture Memory")
    {
        value = memoryData->FreeTexture;
        units = "MB";
    }
    if(type=="Current Graphics API")
    {
        switch(memoryData->Cur3DApi)
        {
        case -1:
            text = "None"; break;
        case 0:
            text = "OpenGL"; break;
        case 1:
            text = "Direct3D8"; break;
        case 2:
            text = "Direct3D9"; break;
        case 3:
            text = "DirectDraw"; break;
        case 4:
            text = "Direct3Dx (old direct3d api)"; break;
        case 5:
            text = "Video Overlay"; break;
        default:
            text = QString("Unknown API (%1)").arg(memoryData->Cur3DApi);
        }
    }
    if(type=="Memory Used")
    {
        value = memoryData->MemUsed;
        units = "MB";
    }
}

void LH_ATITrayToolsData::setTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_ATITrayToolsData::changeTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_index_->setValue(setup_value_type_->value());
}
