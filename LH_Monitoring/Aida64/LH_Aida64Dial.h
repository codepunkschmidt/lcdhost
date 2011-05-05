#ifndef LH_AIDA64DIAL_H
#define LH_AIDA64DIAL_H

#include "LH_Aida64Data.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_Aida64Dial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    LH_Aida64Dial( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_AIDA64DIAL_H
