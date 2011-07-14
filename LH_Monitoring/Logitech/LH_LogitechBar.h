#ifndef LH_LOGITECHBAR_H
#define LH_LOGITECHBAR_H

#include "LH_LogitechData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_LogitechBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_LOGITECHBAR_H
