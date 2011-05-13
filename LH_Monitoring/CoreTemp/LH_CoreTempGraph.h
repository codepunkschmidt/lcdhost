#ifndef LH_CORETEMPGRAPH_H
#define LH_CORETEMPGRAPH_H

#include "LH_CoreTempData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_CoreTempGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_CoreTempGraph();
    static lh_class *classInfo();
};

#endif // LH_CORETEMPGRAPH_H
