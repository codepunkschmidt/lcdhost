#ifndef LH_MONITORINGDIAL_H
#define LH_MONITORINGDIAL_H

#include "../Sources/LH_MonitoringUI.h"
#include "../LH_Dial/LH_Dial.h"
#include <QTime>

class LH_MonitoringDial : public LH_Dial
{
    Q_OBJECT
    QTime pollTimer_;

protected:
    LH_MonitoringUI *ui_;
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;

public:
    LH_MonitoringDial() : LH_Dial(), setup_max_(0), setup_min_(0) {ui_ = NULL;}
    const char *userInit();
    int polling();
    static lh_class *classInfo();

public slots:
    void refresh();
    void updateBounds();
    void updateNeedles();

    void changeAppSelection()  { if(!ui_) return; ui_->changeAppSelection();   updateBounds(); updateNeedles();}
    void setIndexSelection()   { if(!ui_) return; ui_->setIndexSelection();    updateBounds(); updateNeedles();}
    void setTypeSelection()    { if(!ui_) return; ui_->setTypeSelection();     updateBounds(); updateNeedles();}
    void changeTypeSelection() { if(!ui_) return; ui_->changeTypeSelection();  updateBounds(); updateNeedles();}
    void setGroupSelection()   { if(!ui_) return; ui_->setGroupSelection();    updateBounds(); updateNeedles();}
    void changeGroupSelection(){ if(!ui_) return; ui_->changeGroupSelection(); updateBounds(); updateNeedles();}
    void setItemSelection()    { if(!ui_) return; ui_->setItemSelection();     updateBounds(); updateNeedles();}
    void changeItemSelection() { if(!ui_) return; ui_->changeItemSelection();  updateBounds(); updateNeedles();}
};

#endif // LH_MONITORINGDIAL_H
