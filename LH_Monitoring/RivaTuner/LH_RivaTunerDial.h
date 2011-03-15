#ifndef LH_RIVATUNERDIAL_H
#define LH_RIVATUNERDIAL_H

#include "LH_RivaTunerData.h"
#include "../_Templates/LH_MonitoringDial.h"

class LH_RivaTunerDial : public LH_MonitoringDial
{
    Q_OBJECT
public:
    LH_RivaTunerDial( const char *name, LH_QtPlugin *parent );
    static lh_class *classInfo();
};

#endif // LH_RIVATUNERDIAL_H
