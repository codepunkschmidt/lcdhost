#ifndef LH_MONITORINGSOURCE_FRAPS_H
#define LH_MONITORINGSOURCE_FRAPS_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_Fraps : public LH_MonitoringSource
{
    void pingFraps();

    SensorDefinition fps_;

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_Fraps(LH_QtObject *parent);
};


#endif // LH_MONITORINGSOURCE_FRAPS_H
