#ifndef LH_LOGITECHTEXT_H
#define LH_LOGITECHTEXT_H

#include "LH_LogitechData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_LogitechText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_LogitechText();
    static lh_class *classInfo();
};

#endif // LH_LOGITECHTEXT_H
