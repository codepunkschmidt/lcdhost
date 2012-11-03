#ifndef LH_MONITORINGSOURCE_DRIVESTATS_H
#define LH_MONITORINGSOURCE_DRIVESTATS_H

#include "LH_MonitoringSource.h"
#include "LH_MonitoringTypes_DriveStats.h"
#include <QHash>

class LH_MonitoringSource_DriveStats : public LH_MonitoringSource
{
   DrivesList drives_;

   SensorDefinition defBytes_;
   SensorDefinition defPerc_;

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_DriveStats(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_DRIVESTATS_H
