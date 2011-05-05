#ifndef LH_AIDA64TEXT_H
#define LH_AIDA64TEXT_H

#include "LH_Aida64Data.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_Aida64Text : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_Aida64Text( const char *name );
    static lh_class *classInfo();
};

#endif // LH_AIDA64TEXT_H
