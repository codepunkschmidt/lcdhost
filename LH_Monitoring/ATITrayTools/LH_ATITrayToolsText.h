#ifndef LH_ATITRAYTOOLSTEXT_H
#define LH_ATITRAYTOOLSTEXT_H

#include "LH_ATITrayToolsData.h"
#include "../_Templates/LH_MonitoringText.h"

class LH_ATITrayToolsText : public LH_MonitoringText
{
    Q_OBJECT
public:
    LH_ATITrayToolsText( const char *name );
    static lh_class *classInfo();
};

#endif // LH_ATITRAYTOOLSTEXT_H
