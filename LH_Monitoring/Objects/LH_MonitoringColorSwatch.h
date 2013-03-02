#ifndef LH_MONITORINGCOLORSWATCH_H
#define LH_MONITORINGCOLORSWATCH_H

#include "LH_Qt_int.h"
#include "LH_ColorSwatch/LH_ColorSwatch.h"

#include "../Objects/LH_MonitoringObject.h"

class LH_MonitoringColorSwatch : public LH_ColorSwatch, public LH_MonitoringObject
{
    Q_OBJECT

protected:
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;

public:
    LH_MonitoringColorSwatch();
    const char *userInit();
    static lh_class *classInfo();

public slots:
    void updateValue();
    void updateBounds();

    void refreshMonitoringOptions() {
        LH_MonitoringObject::refreshMonitoringOptions();
    }
    void connectChangeEvents() {
        LH_MonitoringObject::connectChangeEvents();
    }
    void changeAppSelection() {
        LH_MonitoringObject::changeAppSelection();
        updateBounds();
    }
    void changeTypeSelection() {
        LH_MonitoringObject::changeTypeSelection();
        updateBounds();
    }
    void changeGroupSelection() {
        LH_MonitoringObject::changeGroupSelection();
        updateBounds();
    }
    void changeItemSelection() {
        LH_MonitoringObject::changeItemSelection();
        updateBounds();
    }
    void dataValidityChanged() {
        LH_MonitoringObject::dataValidityChanged();
    }
};

#endif // LH_MONITORINGCOLORSWATCH_H
