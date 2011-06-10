#ifndef LH_QTPLUGIN_MAILCOUNT_H
#define LH_QTPLUGIN_MAILCOUNT_H

#include "../LH_QtPlugin.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_int.h"

class LH_QtPlugin_Mailcount : public LH_QtPlugin
{
    Q_OBJECT

    LH_Qt_QString *email_addr_;
    LH_Qt_int *email_days_;
    LH_Qt_int *check_interval_;

public:
    LH_QtPlugin_Mailcount();

    const char * lh_name() { return "Mailcount"; }
    const char * lh_shortdesc() { return "Unread mail indicator."; }
    const char * lh_author() { return "Johan \"SirReal\" Lindh"; }
    const char * lh_homepage() { return "<a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a>"; }
    const char * lh_longdesc() { return "Show unread e-mails. Requires an e-mail client with shell support, like Mozilla Thunderbird or Microsoft Outlook."; }
    const lh_blob *lh_logo();
    const char *lh_load();
    void lh_unload();

    virtual const char *init( const char *name, const lh_systemstate* state );
    virtual int notify( int code, void *param );
    virtual void term();
};

#endif // LH_QTPLUGIN_MAILCOUNT_H
