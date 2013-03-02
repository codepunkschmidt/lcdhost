#ifndef LH_MONITORINGGRAPH_H
#define LH_MONITORINGGRAPH_H

#include "LH_Graph/LH_Graph.h"
#include "../Objects/LH_MonitoringObject.h"
#include "../DataCollection/DataCollection.h"

class LH_MonitoringGraph : public LH_Graph, public LH_MonitoringObject
{
    Q_OBJECT
    bool was_empty_;

protected:
    LH_Qt_bool *setup_append_units_;

public:
    LH_MonitoringGraph();

    const char *userInit();
    int notify( int, void* );

    static lh_class *classInfo();
    QImage *render_qimage( int w, int h );

    void updateScale();
    void updateLines();

    void updateDataCache();

    qreal adaptToUnits(qreal val, QString *units = 0, int *prec = 0);

public slots:
    void doInitialize();
    void clearData();
    void updateUnits();
    void configChanged();

    void refreshMonitoringOptions() {
        LH_MonitoringObject::refreshMonitoringOptions();
        updateDataCache();
    }
    void connectChangeEvents() {
        LH_MonitoringObject::connectChangeEvents();
    }
    void changeAppSelection() {
        LH_MonitoringObject::changeAppSelection();
        updateDataCache();
    }
    void changeTypeSelection() {
        LH_MonitoringObject::changeTypeSelection();
        updateDataCache();
    }
    void changeGroupSelection() {
        LH_MonitoringObject::changeGroupSelection();
        updateDataCache();
    }
    void changeItemSelection() {
        LH_MonitoringObject::changeItemSelection();
        updateDataCache();
    }
    void dataValidityChanged() {
        LH_MonitoringObject::dataValidityChanged();
    }
    void renderRequired() {
        requestRender();
    }
};

#endif // LH_MONITORINGGRAPH_H
