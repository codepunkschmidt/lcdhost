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

    void refresh();
    void updateNeedles();

public slots:
    void updateBounds();
    void configChanged()  { updateBounds(); updateNeedles();}

};

#endif // LH_MONITORINGDIAL_H
