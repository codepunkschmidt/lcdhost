#ifndef LH_MONITORINGSOURCE_CORETEMP_H
#define LH_MONITORINGSOURCE_CORETEMP_H

#include "LH_MonitoringSource.h"

class LH_MonitoringSource_CoreTemp : public LH_MonitoringSource
{
    void updateFromArray(QString type, QString name, int count, const uint *valueSet, SensorDefinition def);
    void updateFromArray(QString type, QString name, int count, const qreal *valueSet, SensorDefinition def);

    SensorDefinition temp_;
    SensorDefinition toTj_;
    SensorDefinition perc_;
    SensorDefinition freq_;

protected:
    bool doUpdate();

public:
    LH_MonitoringSource_CoreTemp(LH_QtObject *parent);
};

#endif // LH_MONITORINGSOURCE_CORETEMP_H
