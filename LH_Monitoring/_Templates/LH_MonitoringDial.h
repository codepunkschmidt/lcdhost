#ifndef LH_MONITORINGDIAL_H
#define LH_MONITORINGDIAL_H

#include "LH_MonitoringData.h"
#include "../LH_Dial/LH_Dial.h"
#include <QTime>

class LH_MonitoringDial : public LH_Dial
{
    Q_OBJECT
    QTime pollTimer_;

protected:
    LH_MonitoringData *data_;
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;

public:
    const char *userInit();
    int polling();
    void connect_refresh(QObject* obj);
    static lh_class *classInfo() { return NULL; }

public slots:
    void refresh();
    void updateBounds();
    void updateNeedles();

    void setTypeSelection()    { data_->setTypeSelection();     updateBounds(); updateNeedles(); }
    void changeTypeSelection() { data_->changeTypeSelection();  updateBounds(); updateNeedles(); }
    void setGroupSelection()   { data_->setGroupSelection();    updateBounds(); updateNeedles(); }
    void changeGroupSelection(){ data_->changeGroupSelection(); updateBounds(); updateNeedles(); }
    void setItemSelection()    { data_->setItemSelection();     updateBounds(); updateNeedles(); }
    void changeItemSelection() { data_->changeItemSelection();  updateBounds(); updateNeedles(); }
};

#endif // LH_MONITORINGDIAL_H
