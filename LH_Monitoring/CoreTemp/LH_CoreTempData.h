#ifndef LH_CORETEMPDATA_H
#define LH_CORETEMPDATA_H

#include "../_Templates/LH_MonitoringData.h"
#include <windows.h>

struct CTMemory
{
        unsigned int	uiLoad[256];
        unsigned int	uiTjMax[128];
        unsigned int	uiCoreCnt;
        unsigned int	uiCPUCnt;
        float		fTemp[256];
        float		fVID;
        float		fCPUSpeed;
        float		fFSBSpeed;
        float		fMultiplier;
        char		sCPUName[100];
        unsigned char	ucFahrenheit;
        unsigned char	ucDeltaToTjMax;
};

class LH_CoreTempData: public LH_MonitoringData
{
    QStringList valueTypes;
    float getSelectedValue(int index, int count, float valueSet[]);
    float getSelectedValue(int index, int count, unsigned int valueSet[]);

    void getSelectedValue(CTMemory* ctmemory, float& value, QString& text, QString& units, int& count, int index);
    void loadSensorList(int count, QString type);

    bool is_avg_;
protected:
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    bool getData(float& value, QString& text, QString& units, int& count, int index);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_int *setup_value_type_index_;
    LH_Qt_QStringList *setup_value_index_;
    LH_Qt_int *setup_value_index_index_;

    LH_CoreTempData( LH_QtObject *parent, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );
    ~LH_CoreTempData();

    QString appName() { return "Core Temp"; }
    monitoringDataType getType();
    int getThreshMax();
    bool getCount(int& count);
    bool getNames(QStringList& names);

    void setTypeSelection();
    void setItemSelection();
    void changeTypeSelection();
    void changeItemSelection();
};

#endif // LH_CORETEMPDATA_H
