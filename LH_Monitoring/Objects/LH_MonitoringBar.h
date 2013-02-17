#ifndef LH_MONITORINGBAR_H
#define LH_MONITORINGBAR_H

#include "LH_Qt_int.h"
#include "LH_Bar/LH_Bar.h"

#include "Objects/LH_MonitoringObject.h"

class LH_MonitoringBar : public LH_Bar, public LH_MonitoringObject
{
    Q_OBJECT

protected:
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;
    LH_Qt_QString *setup_minmax_hr_;

public:
    LH_MonitoringBar();
    const char *userInit();
    static lh_class *classInfo();

    QImage *render_qimage( int w, int h );

    void refresh();

public slots:
    void updateBounds();
    void updateBar();

    void refreshMonitoringOptions() {
        LH_MonitoringObject::refreshMonitoringOptions();
    }
    void connectChangeEvents() {
        LH_MonitoringObject::connectChangeEvents();
        updateBounds();
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

#endif // LH_MONITORINGBAR_H
