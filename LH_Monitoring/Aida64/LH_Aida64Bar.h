#ifndef LH_AIDA64BAR_H
#define LH_AIDA64BAR_H

#include "LH_Aida64Data.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_Aida64Bar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_AIDA64BAR_H
