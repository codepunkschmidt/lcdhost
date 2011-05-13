#ifndef LH_GPUZTEXT_H
#define LH_GPUZTEXT_H

#include "LH_GPUZData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_GPUZText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_GPUZText();
    static lh_class *classInfo();
};

#endif // LH_GPUZTEXT_H
