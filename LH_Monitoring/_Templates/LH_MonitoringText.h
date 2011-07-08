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

protected:
    LH_MonitoringData *data_;

    LH_Qt_bool *setup_value_round_;
    LH_Qt_bool *setup_append_units_;
    LH_Qt_QString *setup_pre_text_;
    LH_Qt_QString *setup_post_text_;

public:
    const char *userInit();

    int notify( int, void* );

    static lh_class *classInfo() { return NULL; }

    void connect_updateText(QObject* obj);
    void connect_changeType(QObject* obj);
public slots:
    void updateText(bool rerender = false);
    void changeType();

    void setTypeSelection()    { data_->setTypeSelection();    }
    void changeTypeSelection() { data_->changeTypeSelection();  changeType(); }
    void setGroupSelection()   { data_->setGroupSelection();   }
    void changeGroupSelection(){ data_->changeGroupSelection(); changeType(); }
    void setItemSelection()    { data_->setItemSelection();    }
    void changeItemSelection() { data_->changeItemSelection();  changeType(); }
};

#endif // LH_MONITORINGTEXT_H
