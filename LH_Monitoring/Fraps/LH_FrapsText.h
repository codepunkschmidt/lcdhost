#ifndef LH_FRAPSTEXT_H
#define LH_FRAPSTEXT_H

#include "LH_FrapsData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_FrapsText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_FrapsText();
    static lh_class *classInfo();
};

#endif // LH_FRAPSTEXT_H
