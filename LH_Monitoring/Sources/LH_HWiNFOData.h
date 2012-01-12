#ifndef LH_HWINFODATA_H
#define LH_HWINFODATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_HWiNFOTypes.h"

class LH_HWiNFOData: public LH_MonitoringData
{
    void loadTypesList(_HWiNFO_SENSORS_SHARED_MEM *hwinfoMemory);
    void loadGroupsList(_HWiNFO_SENSORS_SHARED_MEM *hwinfoMemory);
    void loadItemsList(HWiNFO_SENSORS_READING_LIST *list, int max);
    HWiNFO_SENSORS_READING_LIST *getList(HWiNFO_SENSORS_SENSOR_ENTRY, int&);
    void getSelectedValue(HWiNFO_SENSORS_READING_LIST *list, float& value, QString& text, QString& units);

    monitoringDataType getType(QString units);

    int listedType_;
    int listedGroup_;
    QHash<int, int> sensor_indexes_;
    QHash<int, int> item_indexes_;
protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_HWiNFOData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );

    QString appName() { return "HWiNFO"; }
    monitoringDataType getType();
    QString getText(bool addUnits, bool round);
};

#endif // LH_HWINFODATA_H
