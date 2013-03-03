#ifndef LH_MONITORINGSOURCE_AIDA64_H
#define LH_MONITORINGSOURCE_AIDA64_H

#include "LH_MonitoringSource.h"
#include <QDomDocument>

class LH_MonitoringSource_Aida64 : public LH_MonitoringSource
{
    QDomDocument aidaXml_;

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_Aida64(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_AIDA64_H
