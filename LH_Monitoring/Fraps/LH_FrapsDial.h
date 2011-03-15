#ifndef LH_FRAPSDIAL_H
#define LH_FRAPSDIAL_H

#include "LH_FrapsData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_FrapsDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    LH_FrapsDial( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_FRAPSDIAL_H
