#ifndef LH_GPUZGRAPH_H
#define LH_GPUZGRAPH_H

#include "LH_GPUZData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_GPUZGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_GPUZGraph();
    static lh_class *classInfo();
};

#endif // LH_GPUZGRAPH_H
