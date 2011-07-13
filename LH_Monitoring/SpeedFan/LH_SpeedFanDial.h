#ifndef LH_SPEEDFANDIAL_H
#define LH_SPEEDFANDIAL_H

#include "LH_SpeedFanData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_SpeedFanDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_SPEEDFANDIAL_H
