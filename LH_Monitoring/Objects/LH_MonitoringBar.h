#ifndef LH_MONITORINGBAR_H
#define LH_MONITORINGBAR_H

#include "../Sources/LH_MonitoringUI.h"
#include "LH_Qt_int.h"
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

    void refresh();

public slots:
    void updateBounds();
    void configChanged()  { updateBounds(); }
};

#endif // LH_MONITORINGBAR_H
