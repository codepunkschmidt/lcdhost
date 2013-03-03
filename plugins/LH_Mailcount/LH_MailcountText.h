#ifndef LH_MAILCOUNTTEXT_H
#define LH_MAILCOUNTTEXT_H

#include <QTime>
#include <QImage>

#include "../LH_Text/LH_Text.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_int.h"
#include "LH_Qt_bool.h"

class LH_MailcountText: public LH_Text
{
    Q_OBJECT
    LH_Qt_int *email_count_;
    LH_Qt_int *hide_limit_;

public:
    LH_MailcountText();
    static lh_class *classInfo();

public slots:
    void update();
};


#endif // LH_MAILCOUNTTEXT_H
