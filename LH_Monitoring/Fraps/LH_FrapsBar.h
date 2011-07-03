#ifndef LH_FRAPSBAR_H
#define LH_FRAPSBAR_H

#include "LH_FrapsData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_FrapsBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_FrapsBar();
    static lh_class *classInfo();
};

#endif // LH_FRAPSBAR_H
