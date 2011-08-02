#ifndef LH_FRAPSDATA_H
#define LH_FRAPSDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_FrapsTypes.h"

class LH_FrapsData: public LH_MonitoringData
{
    void pingFraps();
protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_FrapsData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false);
    ~LH_FrapsData();

    QString appName() { return "Fraps"; }
    monitoringDataType getType();
};

#endif // LH_FRAPSDATA_H
