#ifndef LH_GPUZDATA_H
#define LH_GPUZDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_GPUZTypes.h"

class LH_GPUZData: public LH_MonitoringData
{
    void getSelectedValue(GPUZ_SH_MEM* gpuzMemory, float& value, QString& text, QString& units);
    void loadTypesList(GPUZ_SH_MEM* gpuzMemory);

protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_GPUZData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );
    ~LH_GPUZData();

    QString appName() { return "GPU-Z"; }
    monitoringDataType getType();
};

#endif // LH_GPUZDATA_H
