#ifndef LH_MAILCOUNTTEXT_H
#define LH_MAILCOUNTTEXT_H

#include <QTime>
#include <QImage>

#include "../LH_Text/LH_Text.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_bool.h"

class LH_MailcountText: public LH_Text
{
    Q_OBJECT
    LH_Qt_QString *email_addr_;
    LH_Qt_int *email_days_;
    LH_Qt_int *check_interval_;
    LH_Qt_bool *hide_zero_;

    int count_;
    int envelope_count_;
    QImage envelope_;
    QTime last_check_;

public:
    LH_MailcountText();

    void input(lh_setup_item *, int, int);

    int notify(int n,void*p);

    int getUnreadMailcount();

    static lh_class *classInfo();

public slots:
    void update();
};


#endif // LH_MAILCOUNTTEXT_H
