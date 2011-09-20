#ifndef LH_DATAVIEWERDIAL_H
#define LH_DATAVIEWERDIAL_H

#include "LH_Dial/LH_Dial.h"

#include "LH_QtPlugin.h"
#include "LH_Qt_int.h"
#include "LH_Qt_QString.h"

#include "LH_DataViewerData.h"

class LH_DataViewerDial : public LH_Dial
{
    Q_OBJECT
    LH_DataViewerData data_;

    qreal barValue_;
    qreal barMin_;
    qreal barMax_;

    qreal toReal(QString str, qreal defaultVal = 0);

protected:
    LH_Qt_QString *setup_lookup_code_;
    LH_Qt_QString *setup_item_name_;
    LH_Qt_QString *setup_value_min_;
    LH_Qt_QString *setup_item_name_min_;
    LH_Qt_QString *setup_value_max_;
    LH_Qt_QString *setup_item_name_max_;

public:
    LH_DataViewerDial();
    QImage *render_qimage( int w, int h );

    int polling();
    static lh_class *classInfo();

public slots:
    void updateDial(bool rerender = false);
};

#endif // LH_DATAVIEWERDIAL_H
