#ifndef LH_SPEEDFANDATA_H
#define LH_SPEEDFANDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_SpeedFanTypes.h"

class LH_SpeedFanData: public LH_MonitoringData
{
    void getSelectedValue(const SFMemory* sfmemory, float& value, QString& text, QString& units, int& count, int index);
    void loadSensorList(const SFMemory* sfmemory);

    bool is_avg_;
protected:
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    bool getData(float& value, QString& text, QString& units, int& count, int index);

public:

    LH_SpeedFanData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );

    QString appName() { return "SpeedFan"; }
    monitoringDataType getType();
    int getThreshMax();
    bool getCount(int& count);
    bool getNames(QStringList& names);
    bool getDeadValue(float& value);
    bool getDeadValue_Transformed(float& value);
};

#endif // LH_SPEEDFANDATA_H
