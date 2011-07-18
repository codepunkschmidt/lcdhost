#ifndef LH_LOGITECHTEXT_H
#define LH_LOGITECHTEXT_H

#include "../_Templates/LH_MonitoringText.h"

class LH_LogitechText : public LH_MonitoringText
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_LOGITECHTEXT_H
