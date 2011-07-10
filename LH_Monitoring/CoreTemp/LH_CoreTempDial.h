#ifndef LH_CORETEMPDIAL_H
#define LH_CORETEMPDIAL_H

#include "LH_CoreTempData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_CoreTempDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    LH_CoreTempDial();
    static lh_class *classInfo();
};

#endif // LH_CORETEMPDIAL_H
