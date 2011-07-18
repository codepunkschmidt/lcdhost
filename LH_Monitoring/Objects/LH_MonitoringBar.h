#ifndef LH_MONITORINGBAR_H
#define LH_MONITORINGBAR_H

#include "../Sources/LH_MonitoringUI.h"
#include "../LH_Qt_int.h"
#include "../LH_Bar/LH_Bar.h"

class LH_MonitoringBar : public LH_Bar
{
    Q_OBJECT

protected:
    LH_MonitoringUI *ui_;
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;

public:
    LH_MonitoringBar();

    const char *userInit();
    int notify( int, void* );
    QImage *render_qimage( int w, int h );
    static lh_class *classInfo();

public slots:
    void refresh();
    void updateBounds();

    void changeAppSelection()  { if(!ui_) return; ui_->changeAppSelection();   updateBounds(); }
    void setIndexSelection()   { if(!ui_) return; ui_->setIndexSelection();    updateBounds(); }
    void setTypeSelection()    { if(!ui_) return; ui_->setTypeSelection();     updateBounds(); }
    void changeTypeSelection() { if(!ui_) return; ui_->changeTypeSelection();  updateBounds(); }
    void setGroupSelection()   { if(!ui_) return; ui_->setGroupSelection();    updateBounds(); }
    void changeGroupSelection(){ if(!ui_) return; ui_->changeGroupSelection(); updateBounds(); }
    void setItemSelection()    { if(!ui_) return; ui_->setItemSelection();     updateBounds(); }
    void changeItemSelection() { if(!ui_) return; ui_->changeItemSelection();  updateBounds(); }
};

#endif // LH_MONITORINGBAR_H
