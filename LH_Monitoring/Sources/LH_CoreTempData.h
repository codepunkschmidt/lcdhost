#ifndef LH_CORETEMPDATA_H
#define LH_CORETEMPDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_CoreTempTypes.h"

class LH_CoreTempData: public LH_MonitoringData
{
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
    LH_CoreTempData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );

    QString appName() { return "Core Temp"; }
    monitoringDataType getType();
    int getThreshMax();
    bool getCount(int& count);
    bool getNames(QStringList& names);
};

#endif // LH_CORETEMPDATA_H
