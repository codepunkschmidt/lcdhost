#ifndef LH_FRAPSGRAPH_H
#define LH_FRAPSGRAPH_H

#include "LH_FrapsData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_FrapsGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_FRAPSGRAPH_H
