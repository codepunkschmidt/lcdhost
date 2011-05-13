#ifndef LH_SPEEDFANBAR_H
#define LH_SPEEDFANBAR_H

#include "LH_SpeedFanData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_SpeedFanBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_SpeedFanBar();
    static lh_class *classInfo();
};

#endif // LH_SPEEDFANBAR_H
