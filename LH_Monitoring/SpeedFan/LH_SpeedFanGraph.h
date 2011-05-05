#ifndef LH_SPEEDFANGRAPH_H
#define LH_SPEEDFANGRAPH_H

#include "LH_SpeedFanData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_SpeedFanGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_SpeedFanGraph( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_SPEEDFANGRAPH_H
