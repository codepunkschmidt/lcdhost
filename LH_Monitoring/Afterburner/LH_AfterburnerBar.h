#ifndef LH_AFTERBURNERBAR_H
#define LH_AFTERBURNERBAR_H

#include "LH_AfterburnerData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_AfterburnerBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_AFTERBURNERBAR_H
