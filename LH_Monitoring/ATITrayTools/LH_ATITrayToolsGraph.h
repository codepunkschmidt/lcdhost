#ifndef LH_ATITRAYTOOLSGRAPH_H
#define LH_ATITRAYTOOLSGRAPH_H

#include "LH_ATITrayToolsData.h"
#include "../_Templates/LH_MonitoringGraph.h"

class LH_ATITrayToolsGraph : public LH_MonitoringGraph
{
    Q_OBJECT
public:
    LH_ATITrayToolsGraph( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_ATITRAYTOOLSGRAPH_H
