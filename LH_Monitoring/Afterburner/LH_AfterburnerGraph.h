#ifndef LH_AFTERBURNERGRAPH_H
#define LH_AFTERBURNERGRAPH_H

#include "LH_AfterburnerData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_AfterburnerGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_AfterburnerGraph();
    static lh_class *classInfo();
};

#endif // LH_AFTERBURNERGRAPH_H
