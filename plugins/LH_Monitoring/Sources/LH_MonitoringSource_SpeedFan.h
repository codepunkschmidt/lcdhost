#ifndef LH_MONITORINGSOURCE_SPEEDFAN_H
#define LH_MONITORINGSOURCE_SPEEDFAN_H

#include "LH_MonitoringSource.h"


class LH_MonitoringSource_SpeedFan : public LH_MonitoringSource
{
    void updateFromArray(QString type, int count, const float *valueSet, float modifier, SensorDefinition def);
    void updateFromArray(QString type, int count, const uint  *valueSet, float modifier, SensorDefinition def);
    void updateFromArray(QString type, int count, const int   *valueSet, float modifier, SensorDefinition def);

    SensorDefinition temp_;
    SensorDefinition fans_;
    SensorDefinition volt_;

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_SpeedFan(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_SPEEDFAN_H
