#ifndef LH_MONITORINGSOURCE_ATITRAYTOOLS_H
#define LH_MONITORINGSOURCE_ATITRAYTOOLS_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_ATITrayTools : public LH_MonitoringSource
{
protected:
    bool doUpdate();

public:
    LH_MonitoringSource_ATITrayTools(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_ATITRAYTOOLS_H
