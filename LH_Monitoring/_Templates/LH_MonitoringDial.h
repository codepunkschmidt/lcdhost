#ifndef LH_MONITORINGDIAL_H
#define LH_MONITORINGDIAL_H

#include "LH_MonitoringData.h"
#include "..\LH_Dial\LH_Dial.h"

class LH_MonitoringDial : public LH_Dial
{
    Q_OBJECT
protected:
    LH_MonitoringData *data_;
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;
public:
    LH_MonitoringDial(const char *name, LH_QtPlugin *parent);
    ~LH_MonitoringDial();

    int polling();

    static lh_class *classInfo() { return NULL; }

    void connect_refresh(QObject* obj);
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
