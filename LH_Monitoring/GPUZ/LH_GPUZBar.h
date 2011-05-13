#ifndef LH_GPUZBAR_H
#define LH_GPUZBAR_H

#include "LH_GPUZData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_GPUZBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_GPUZBar();
    static lh_class *classInfo();
};

#endif // LH_GPUZBAR_H
