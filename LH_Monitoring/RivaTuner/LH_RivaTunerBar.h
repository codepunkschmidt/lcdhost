#ifndef LH_RIVATUNERBAR_H
#define LH_RIVATUNERBAR_H

#include "LH_RivaTunerData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_RivaTunerBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_RivaTunerBar();
    static lh_class *classInfo();
};

#endif // LH_RIVATUNERBAR_H
