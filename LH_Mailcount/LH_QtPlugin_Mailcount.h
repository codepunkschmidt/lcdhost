#ifndef LH_QTPLUGIN_MAILCOUNT_H
#define LH_QTPLUGIN_MAILCOUNT_H

#include <QTime>
#include "../LH_QtPlugin.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"

class LH_QtPlugin_Mailcount : public LH_QtPlugin
{
    Q_OBJECT

    static LH_QtPlugin_Mailcount *instance_;

    LH_Qt_int *email_count_;
    LH_Qt_QString *email_addr_;
    LH_Qt_int *email_days_;
    LH_Qt_int *check_interval_;

    QTime last_check_;

public:
    LH_QtPlugin_Mailcount();
    ~LH_QtPlugin_Mailcount();

    virtual const char *init( lh_callback_t cb, int cb_id, const char *name, const lh_systemstate* state );
    virtual int notify( int code, void *param );
    virtual void term();

    int getUnreadMailcount();

    static LH_QtPlugin_Mailcount *instance() { return instance_; }
};

#endif // LH_QTPLUGIN_MAILCOUNT_H
