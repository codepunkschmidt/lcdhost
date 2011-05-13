#ifndef LH_CORETEMPTEXT_H
#define LH_CORETEMPTEXT_H

#include "LH_CoreTempData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_CoreTempText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_CoreTempText();
    static lh_class *classInfo();
};

#endif // LH_CORETEMPTEXT_H
