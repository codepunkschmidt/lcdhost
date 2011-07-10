#ifndef LH_RIVATUNERGRAPH_H
#define LH_RIVATUNERGRAPH_H

#include "LH_RivaTunerData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_RivaTunerGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_RivaTunerGraph();
    static lh_class *classInfo();
};

#endif // LH_RIVATUNERGRAPH_H
