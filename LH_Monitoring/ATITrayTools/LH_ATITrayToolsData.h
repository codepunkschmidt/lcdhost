#ifndef LH_ATITRAYTOOLSDATA_H
#define LH_ATITRAYTOOLSDATA_H

#include "../_Templates/LH_MonitoringData.h"
#ifdef Q_WS_WIN
# include <windows.h>
#endif

#ifndef DWORD
# define DWORD quint32
#endif

struct tagTATTData
{
     DWORD CurGPU;      //Current GPU Speed
     DWORD CurMEM;      //Current MEM Speed
     DWORD isGameActive; //If game from profile is active, this field will be 1 or 0 if not.
     DWORD is3DActive;  //1=3D mode, 0=2D mode

     DWORD isTempMonSupported; //1 - if temperature monitoring supported by ATT
     DWORD GPUTemp;            //GPU Temperature
     DWORD ENVTemp;            //ENV Temperature
     DWORD FanDuty;            //FAN Duty

     DWORD MAXGpuTemp;         //MAX GPU Temperature
     DWORD MINGpuTemp;         //MIN GPU Temperature
     DWORD MAXEnvTemp;         //MAX ENV Temperature
     DWORD MINEnvTemp;         //MIN ENV Temperature
                               //3d settings
     DWORD CurD3DAA;           //Direct3D Antialiasing value
     DWORD CurD3DAF;           //Direct3D Anisotropy value
     DWORD CurOGLAA;           //OpenGL Antialiasing value
     DWORD CurOGLAF;           //OpenGL Anisotropy value
     DWORD IsActive;           //is 3d application active
     DWORD CurFPS;                     // current FPS

     DWORD FreeVideo;  //Free Video Memory
     DWORD FreeTexture;  //Free Texture Memory
     DWORD Cur3DApi;    //Current API used in applciation
                    // 0 - OpenGL
                    // 1 - Direct3D8
                    // 2 - Direct3D9
                    // 3 - DirectDraw
                    // 4 - Direct3Dx (old direct3d api)
                    // 5 - Video Overlay
                    // -1 - No active rendering application
     DWORD MemUsed;
};

class LH_ATITrayToolsData: public LH_MonitoringData
{
    QStringList valueTypes;
    void getSelectedValue(tagTATTData* memoryData, float& value, QString& text, QString& units);

protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_int *setup_value_type_index_;

    LH_ATITrayToolsData( LH_QtObject *parent, monitoringDataMode dataMode = mdmNumbers );
    ~LH_ATITrayToolsData();
    QString appName() { return "ATI Tray Tools"; }

    void setTypeSelection();
    void changeTypeSelection();
};

#endif // LH_ATITRAYTOOLSDATA_H
