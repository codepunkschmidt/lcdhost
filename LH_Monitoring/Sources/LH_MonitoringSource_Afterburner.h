#ifndef LH_MONITORINGSOURCE_AFTERBURNER_H
#define LH_MONITORINGSOURCE_AFTERBURNER_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_Afterburner : public LH_MonitoringSource
{
protected:
    bool doUpdate();

public:
    LH_MonitoringSource_Afterburner(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_AFTERBURNER_H
