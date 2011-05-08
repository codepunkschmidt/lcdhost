#ifndef LH_AFTERBURNERBAR_H
#define LH_AFTERBURNERBAR_H

#include "LH_AfterburnerData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_AfterburnerBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_AfterburnerBar( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_AFTERBURNERBAR_H
