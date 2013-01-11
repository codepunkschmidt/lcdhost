#ifndef LH_MONITORINGPIE_H
#define LH_MONITORINGPIE_H

#include "LH_Dial/LH_Dial.h"
#include <QTime>

class LH_MonitoringPie : public LH_Dial
{
    Q_OBJECT
    QTime pollTimer_;

protected:
    LH_MonitoringUI *ui_;

public:
    LH_MonitoringPie() : LH_Dial(DIALTYPE_PIE) {ui_ = NULL;}
    const char *userInit();
    int polling();
    static lh_class *classInfo();

    void refresh();
    void updateNeedles();

public slots:
    void updateBounds();
    void configChanged();

};

#endif // LH_MONITORINGDIAL_H
