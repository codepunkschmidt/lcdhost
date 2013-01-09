#ifndef LH_MONITORINGSOURCE_GPUZ_H
#define LH_MONITORINGSOURCE_GPUZ_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_GPUZ : public LH_MonitoringSource
{
protected:
    bool doUpdate();

public:
    LH_MonitoringSource_GPUZ(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_GPUZ_H
