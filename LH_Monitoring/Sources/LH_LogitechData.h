#ifndef LH_LOGITECHDATA_H
#define LH_LOGITECHDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"

class LH_LogitechData: public LH_MonitoringData
{
    QList<QStringList> batteryStates;

protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_LogitechData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false);
    ~LH_LogitechData();

    QString appName() { return "Logitech Monitoring Gadget"; }
    monitoringDataType getType();
};

#endif // LH_LOGITECHDATA_H
