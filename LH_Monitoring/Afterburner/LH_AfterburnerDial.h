#ifndef LH_AFTERBURNERDIAL_H
#define LH_AFTERBURNERDIAL_H

#include "LH_AfterburnerData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_AfterburnerDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    LH_AfterburnerDial();
    static lh_class *classInfo();
};

#endif // LH_AFTERBURNERDIAL_H
