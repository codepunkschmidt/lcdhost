#ifndef LH_MONITORINGGRAPH_H
#define LH_MONITORINGGRAPH_H

#include "LH_MonitoringData.h"
#include "../LH_Graph/LH_Graph.h"

class LH_MonitoringGraph : public LH_Graph
{
    Q_OBJECT
    bool was_empty_;

protected:
    LH_MonitoringData *data_;
    LH_Qt_bool *setup_append_units_;

public:
    LH_MonitoringGraph() : LH_Graph(0,2), was_empty_(true), data_(0), setup_append_units_(0) {}

    const char *userInit();
    int notify( int, void* );

    static lh_class *classInfo() { return NULL; }
    QImage *render_qimage( int w, int h );

    void updateScale();
    void updateLines();
    void connect_clearData(QObject* obj);
public slots:
    void clearData();
    void updateUnits();

    void setTypeSelection()    { data_->setTypeSelection();     }
    void changeTypeSelection() { data_->changeTypeSelection();  }
    void setGroupSelection()   { data_->setGroupSelection();    }
    void changeGroupSelection(){ data_->changeGroupSelection(); }
    void setItemSelection()    { data_->setItemSelection();     }
    void changeItemSelection() { data_->changeItemSelection();  }
};

#endif // LH_MONITORINGGRAPH_H
