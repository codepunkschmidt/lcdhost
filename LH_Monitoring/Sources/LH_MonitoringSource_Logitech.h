#ifndef LH_MONITORINGSOURCE_LOGITECH_H
#define LH_MONITORINGSOURCE_LOGITECH_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_Logitech : public LH_MonitoringSource
{
    QList<QStringList> batteryStates_;
    SensorDefinition pcnt_;

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_Logitech(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_LOGITECH_H
