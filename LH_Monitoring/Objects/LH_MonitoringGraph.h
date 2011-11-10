#ifndef LH_MONITORINGGRAPH_H
#define LH_MONITORINGGRAPH_H

#include "Sources/LH_MonitoringUI.h"
#include "LH_Graph/LH_Graph.h"

class LH_MonitoringGraph : public LH_Graph
{
    Q_OBJECT
    bool was_empty_;

protected:
    LH_MonitoringUI *ui_;
    LH_Qt_bool *setup_append_units_;

public:
    LH_MonitoringGraph() : LH_Graph(0,2), was_empty_(true), setup_append_units_(0) {ui_ = NULL;}

    const char *userInit();
    int notify( int, void* );

    static lh_class *classInfo();
    QImage *render_qimage( int w, int h );

    void updateScale();
    void updateLines();

public slots:
    void clearData();
    void updateUnits();

    void configChanged()  { clearData(); updateUnits();}
};

#endif // LH_MONITORINGGRAPH_H
