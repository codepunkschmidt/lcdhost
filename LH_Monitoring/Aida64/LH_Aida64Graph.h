#ifndef LH_AIDA64GRAPH_H
#define LH_AIDA64GRAPH_H

#include "LH_Aida64Data.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_Aida64Graph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_Aida64Graph();
    static lh_class *classInfo();
};

#endif // LH_AIDA64GRAPH_H
