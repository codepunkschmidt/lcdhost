#ifndef LH_ATITRAYTOOLSDATA_H
#define LH_ATITRAYTOOLSDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_ATITrayToolsTypes.h"

class LH_ATITrayToolsData: public LH_MonitoringData
{
    void getSelectedValue(tagTATTData* memoryData, float& value, QString& text, QString& units);

protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_ATITrayToolsData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );

    QString appName() { return "ATI Tray Tools"; }
};

#endif // LH_ATITRAYTOOLSDATA_H
