#ifndef LH_MONITORINGBAR_H
#define LH_MONITORINGBAR_H

#include "LH_MonitoringData.h"
#include "../LH_Bar/LH_Bar.h"

class LH_MonitoringBar : public LH_Bar
{
    Q_OBJECT
protected:
    LH_MonitoringData *data_;
    LH_Qt_int *setup_max_;
    LH_Qt_int *setup_min_;
public:
    LH_MonitoringBar(const char *name, LH_QtPlugin *parent);
    ~LH_MonitoringBar();

    int notify( int, void* );

    static lh_class *classInfo() { return NULL; }
    QImage *render_qimage( int w, int h );

    void connect_refresh(QObject* obj);
public slots:
    void refresh();
    void updateBounds();

    void setTypeSelection()    { data_->setTypeSelection();     updateBounds();}
    void changeTypeSelection() { data_->changeTypeSelection();  updateBounds();}
    void setGroupSelection()   { data_->setGroupSelection();    updateBounds();}
    void changeGroupSelection(){ data_->changeGroupSelection(); updateBounds();}
    void setItemSelection()    { data_->setItemSelection();     updateBounds();}
    void changeItemSelection() { data_->changeItemSelection();  updateBounds();}
};

#endif // LH_MONITORINGBAR_H
