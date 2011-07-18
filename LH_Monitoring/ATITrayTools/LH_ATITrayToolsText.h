#ifndef LH_ATITRAYTOOLSTEXT_H
#define LH_ATITRAYTOOLSTEXT_H

#include "../_Templates/LH_MonitoringText.h"

class LH_ATITrayToolsText : public LH_MonitoringText
{
    Q_OBJECT
public:
    const char *userInit();
    static lh_class *classInfo();
};

#endif // LH_ATITRAYTOOLSTEXT_H
