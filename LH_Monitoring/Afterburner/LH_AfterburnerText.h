#ifndef LH_AFTERBURNERTEXT_H
#define LH_AFTERBURNERTEXT_H

#include "../_Templates/LH_MonitoringText.h"

class LH_AfterburnerText : public LH_MonitoringText
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_AFTERBURNERTEXT_H
