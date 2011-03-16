#ifndef LH_ATITRAYTOOLSBAR_H
#define LH_ATITRAYTOOLSBAR_H

#include "LH_ATITrayToolsData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_ATITrayToolsBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_ATITrayToolsBar(const char *name, LH_QtPlugin *parent);
    static lh_class *classInfo();
};

#endif // LH_ATITRAYTOOLSBAR_H
