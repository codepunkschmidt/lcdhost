#ifndef LH_MONITORINGTEXT_H
#define LH_MONITORINGTEXT_H

#include "../LH_Text/LH_Text.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_float.h"
#include <QFontMetrics>
#include "LH_MonitoringData.h"

class LH_MonitoringText : public LH_Text
{
    Q_OBJECT

    void setVisibility(LH_Qt_int* qt, bool Hidden, int minValue, int maxValue);
    void setVisibility(LH_Qt_float* qt, bool Hidden, float minValue, float maxValue);

protected:
    QString statusCode_;

    LH_MonitoringData *data_;

    LH_Qt_bool *setup_value_round_;
    LH_Qt_bool *setup_append_units_;
    LH_Qt_QString *setup_pre_text_;
    LH_Qt_QString *setup_post_text_;

    LH_Qt_bool *setup_use_thresh_pen_colors_;
    LH_Qt_bool *setup_use_thresh_bg_colors_;

    LH_Qt_QColor *setup_thresh_vlo_color_pen_;
    LH_Qt_QColor *setup_thresh_lo_color_pen_;
    LH_Qt_QColor *setup_thresh_ok_color_pen_;
    LH_Qt_QColor *setup_thresh_hi_color_pen_;
    LH_Qt_QColor *setup_thresh_vhi_color_pen_;

    LH_Qt_QColor *setup_thresh_vlo_color_bg_;
    LH_Qt_QColor *setup_thresh_lo_color_bg_;
    LH_Qt_QColor *setup_thresh_ok_color_bg_;
    LH_Qt_QColor *setup_thresh_hi_color_bg_;
    LH_Qt_QColor *setup_thresh_vhi_color_bg_;

    LH_Qt_int *setup_thresh_vlo_int_;
    LH_Qt_int *setup_thresh_lo_int_;
    LH_Qt_int *setup_thresh_hi_int_;
    LH_Qt_int *setup_thresh_vhi_int_;

    LH_Qt_float *setup_thresh_vlo_float_;
    LH_Qt_float *setup_thresh_lo_float_;
    LH_Qt_float *setup_thresh_hi_float_;
    LH_Qt_float *setup_thresh_vhi_float_;

    void createThresholdSet(LH_Qt_QColor **color_pen, LH_Qt_QColor **color_bg,
                            QString title, QColor pen_color, QColor bg_color, bool low_value,
                            LH_Qt_float **thresh_flt = 0, LH_Qt_int **thresh_int = 0 );
public:
    LH_MonitoringText();
    ~LH_MonitoringText();

    QColor pencolor();
    QColor bgcolor();

    int notify( int, void* );

    static lh_class *classInfo() { return NULL; }

    void connect_updateText(QObject* obj);
    void connect_changeType(QObject* obj);
public slots:
    void updateText(bool rerender = false);
    void changeType();

    void setTypeSelection()    { data_->setTypeSelection();    }
    void changeTypeSelection() {
        data_->changeTypeSelection();
        changeType();
    }
    void setGroupSelection()   { data_->setGroupSelection();   }
    void changeGroupSelection(){
        data_->changeGroupSelection();
        changeType();
    }
    void setItemSelection()    { data_->setItemSelection();    }
    void changeItemSelection() {
        data_->changeItemSelection();
        changeType();
    }

    void syncThresh_Int();
    void syncThresh_Float();
};

#endif // LH_MONITORINGTEXT_H
