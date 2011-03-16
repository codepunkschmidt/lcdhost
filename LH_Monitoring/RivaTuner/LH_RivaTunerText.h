#ifndef LH_RivaTunerTEXT_H
#define LH_RivaTunerTEXT_H

#include "LH_RivaTunerData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_RivaTunerText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_RivaTunerText( const char *name );
    static lh_class *classInfo();
};

#endif // LH_RivaTunerTEXT_H
