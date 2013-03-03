#ifndef LH_MONITORINGSOURCE_HWINFO_H
#define LH_MONITORINGSOURCE_HWINFO_H

#include "LH_MonitoringSource.h"
#include "LH_MonitoringTypes_HWiNFO.h"

class LH_MonitoringSource_HWiNFO : public LH_MonitoringSource
{
    void updateReadingListValues(QString SensorName, QString TypeName, HWiNFO_SENSORS_READING_LIST* items, int length, bool aggregate = false);

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_HWiNFO(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_HWINFO_H
