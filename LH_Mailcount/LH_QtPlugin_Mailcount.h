#ifndef LH_QTPLUGIN_MAILCOUNT_H
#define LH_QTPLUGIN_MAILCOUNT_H

#include <QTime>
#include "../LH_QtPlugin.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"

class LH_QtPlugin_Mailcount : public LH_QtPlugin
{
    Q_OBJECT

    LH_Qt_int *email_count_;
    LH_Qt_QString *email_addr_;
    LH_Qt_int *email_days_;
    LH_Qt_int *check_interval_;
    LH_Qt_int *manual_adjust_;

    QTime last_check_;

public:
    virtual const char *userInit();
    virtual int notify( int code, void *param );
    virtual void userTerm();

public slots:
    void getUnreadMailcount();
};

#endif // LH_QTPLUGIN_MAILCOUNT_H
