#ifndef LH_SPEEDFANTEXT_H
#define LH_SPEEDFANTEXT_H

#include "LH_SpeedFanData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_SpeedFanText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_SpeedFanText();
    static lh_class *classInfo();
};

#endif // LH_SPEEDFANTEXT_H
