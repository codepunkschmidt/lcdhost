#ifndef LH_MONITORINGSOURCE_HWMONITOR_H
#define LH_MONITORINGSOURCE_HWMONITOR_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_HWMonitor : public LH_MonitoringSource
{
protected:
    bool doUpdate();

public:
    LH_MonitoringSource_HWMonitor(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_HWMONITOR_H
