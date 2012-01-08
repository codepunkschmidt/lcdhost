#ifndef LH_MONITORINGTEXT_H
#define LH_MONITORINGTEXT_H

#include "LH_Text/LH_Text.h"
#include "LH_Qt_int.h"
#include "LH_Qt_float.h"
#include <QFontMetrics>
#include "Sources/LH_MonitoringUI.h"

class LH_MonitoringText : public LH_Text
{
    Q_OBJECT

protected:
    LH_MonitoringUI *ui_;
    LH_Qt_bool *setup_value_round_;
    LH_Qt_bool *setup_append_units_;
    LH_Qt_QString *setup_pre_text_;
    LH_Qt_QString *setup_post_text_;

public:
    LH_MonitoringText();

    const char *userInit();
    int notify( int, void* );

    static lh_class *classInfo();

public slots:
    void updateText(bool rerender = false);
    void configChanged()  { updateText(); }

};

#endif // LH_MONITORINGTEXT_H
