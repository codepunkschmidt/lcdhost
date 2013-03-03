#ifndef LH_MONITORINGDIAL_H
#define LH_MONITORINGDIAL_H

#include "LH_Dial/LH_Dial.h"
#include <QTime>

#include "../Objects/LH_MonitoringObject.h"

class LH_MonitoringDial : public LH_Dial, public LH_MonitoringObject
{
    Q_OBJECT

protected:
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;
    LH_Qt_QString *setup_minmax_hr_;

public:
    LH_MonitoringDial();
    const char *userInit();

    static lh_class *classInfo();

    void refresh();
    void updateNeedlesList(QStringList *names = NULL);

public slots:
    void updateBounds();
    void updateNeedles();

    void refreshMonitoringOptions() {
        LH_MonitoringObject::refreshMonitoringOptions();
    }
    void connectChangeEvents() {
        LH_MonitoringObject::connectChangeEvents();
        updateBounds(); updateNeedlesList();
    }
    void changeAppSelection() {
        LH_MonitoringObject::changeAppSelection();
        updateBounds(); updateNeedlesList();
    }
    void changeTypeSelection() {
        LH_MonitoringObject::changeTypeSelection();
        updateBounds(); updateNeedlesList();
    }
    void changeGroupSelection() {
        LH_MonitoringObject::changeGroupSelection();
        updateBounds(); updateNeedlesList();
    }
    void changeItemSelection() {
        LH_MonitoringObject::changeItemSelection();
        updateBounds(); updateNeedlesList();
    }
    void dataValidityChanged() {
        LH_MonitoringObject::dataValidityChanged();
    }
};

#endif // LH_MONITORINGDIAL_H
