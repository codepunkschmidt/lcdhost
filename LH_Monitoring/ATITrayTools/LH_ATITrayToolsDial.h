#ifndef LH_ATITRAYTOOLSDIAL_H
#define LH_ATITRAYTOOLSDIAL_H

#include "LH_ATITrayToolsData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_ATITrayToolsDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    LH_ATITrayToolsDial();
    static lh_class *classInfo();
};

#endif // LH_ATITRAYTOOLSDIAL_H
