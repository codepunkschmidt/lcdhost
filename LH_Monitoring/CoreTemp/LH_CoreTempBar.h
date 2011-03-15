#ifndef LH_CORETEMPBAR_H
#define LH_CORETEMPBAR_H

#include "LH_CoreTempData.h"
#include "../_Templates/LH_MonitoringBar.h"

class LH_CoreTempBar : public LH_MonitoringBar
{
    Q_OBJECT
public:
    LH_CoreTempBar( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_CORETEMPBAR_H
