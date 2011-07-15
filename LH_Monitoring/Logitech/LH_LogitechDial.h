#ifndef LH_LOGITECHDIAL_H
#define LH_LOGITECHDIAL_H

#include "LH_LogitechData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_LogitechDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_LOGITECHDIAL_H
