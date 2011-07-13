#ifndef LH_GPUZDIAL_H
#define LH_GPUZDIAL_H

#include "LH_GPUZData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_GPUZDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_GPUZDIAL_H
