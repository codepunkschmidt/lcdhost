#include "LH_ATITrayToolsData.h"

LH_ATITrayToolsData::LH_ATITrayToolsData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode )
{
    Q_UNUSED(includeGroups);
    ui_->reset();
    ui_->setVisible(mon_item);
    if(dataMode & mdmNumbers) ui_->append(mon_item,"GPU Speed");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"MEM Speed");
    if(dataMode & mdmText) ui_->append(mon_item,"Game Profile Active");        // 1 = Yes; 0 = No
    if(dataMode & mdmText) ui_->append(mon_item,"3D Active");                  // 1 = 3D;  0 = 2D
    if(dataMode & mdmText) ui_->append(mon_item,"Temp Mon Supported");         // 1 = Yes;  0 = No
    if(dataMode & mdmNumbers) ui_->append(mon_item,"GPU Temp");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"ENV Temp");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"FAN Duty");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"MAX GPU Temp");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"MIN GPU Temp");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"MAX ENV Temp");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"MIN ENV Temp");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"Direct3D Antialiasing");   // What values can this have???
    if(dataMode & mdmNumbers) ui_->append(mon_item,"Direct3D Anisotropy");     // What values can this have???
    if(dataMode & mdmNumbers) ui_->append(mon_item,"OpenGL Antialiasing");     // What values can this have???
    if(dataMode & mdmNumbers) ui_->append(mon_item,"OpenGL Anisotropy");       // What values can this have???
    if(dataMode & mdmText) ui_->append(mon_item,"3D Active");                  // 1 = Yes;  0 = No (I think)
    if(dataMode & mdmNumbers) ui_->append(mon_item,"FPS");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"Free Video Memory");
    if(dataMode & mdmNumbers) ui_->append(mon_item,"Free Texture Memory");
    if(dataMode & mdmText) ui_->append(mon_item,"Current Graphics API");       // 0 - OpenGL, 1 - Direct3D8, 2 - Direct3D9, 3 - DirectDraw, 4 - Direct3Dx (old direct3d api), 5 - Video Overlay, -1 - No active rendering application
    if(dataMode & mdmNumbers) ui_->append(mon_item,"Memory Used");
    ui_->refresh(mon_item);
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

    ui_->setReadonly(mon_item, !resultVal);
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
    QString type = ui_->valueText(mon_item);

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
