#ifndef LH_MONITORINGSOURCE_RIVATUNER_H
#define LH_MONITORINGSOURCE_RIVATUNER_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_RivaTuner : public LH_MonitoringSource
{
protected:
    bool doUpdate();

public:
    LH_MonitoringSource_RivaTuner(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_RIVATUNER_H
