#ifndef LH_MONITORINGGRAPH_H
#define LH_MONITORINGGRAPH_H

#include "../Sources/LH_MonitoringUI.h"
#include "../LH_Graph/LH_Graph.h"

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

    void changeAppSelection()  { if(!ui_) return; ui_->changeAppSelection();   clearData(); updateUnits();}
    void setIndexSelection()   { if(!ui_) return; ui_->setIndexSelection();    clearData(); updateUnits();}
    void setTypeSelection()    { if(!ui_) return; ui_->setTypeSelection();     clearData(); updateUnits();}
    void changeTypeSelection() { if(!ui_) return; ui_->changeTypeSelection();  clearData(); updateUnits();}
    void setGroupSelection()   { if(!ui_) return; ui_->setGroupSelection();    clearData(); updateUnits();}
    void changeGroupSelection(){ if(!ui_) return; ui_->changeGroupSelection(); clearData(); updateUnits();}
    void setItemSelection()    { if(!ui_) return; ui_->setItemSelection();     clearData(); updateUnits();}
    void changeItemSelection() { if(!ui_) return; ui_->changeItemSelection();  clearData(); updateUnits();}
};

#endif // LH_MONITORINGGRAPH_H
