#ifndef LH_DRIVESTATSDATA_H
#define LH_DRIVESTATSDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"

#include "LH_DriveStatsTypes.h"

class LH_DriveStatsData: public LH_MonitoringData
{
    DriveInfo driveInfo;
    void parseBytes(qlonglong bytes, float& value, QString& text, QString& units);

protected:
    bool getData(float& value, QString& text, QString& units);
    bool getPieMax(float &value);

public:
    LH_DriveStatsData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false);

    QString appName() { return "Drive Stats"; }
    monitoringDataType getType();
};

#endif // LH_DRIVESTATSDATA_H
