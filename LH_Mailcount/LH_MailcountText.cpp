#include "LH_MailcountText.h"

LH_PLUGIN_CLASS(LH_MailcountText)

lh_class *LH_MailcountText::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "System/Mail",
        "SystemMailText",
        "Unread mail (Text)",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_MailcountText::LH_MailcountText() : LH_Text()
{
    count_ = 0;
    setup_text_->setName( "Unread Mail Count" );
    setup_text_->setValue(" ");
    setup_text_->setFlag( LH_FLAG_READONLY, true );
    setup_text_->setFlag( LH_FLAG_NOSAVE, true );
    email_addr_ = new LH_Qt_QString(this,tr("Only check address"),QString(),LH_FLAG_AUTORENDER);
    email_days_ = new LH_Qt_int(this,tr("Days back to check"),7,LH_FLAG_AUTORENDER);
    hide_zero_ = new LH_Qt_bool(this,tr("Hide when zero"),false,LH_FLAG_AUTORENDER);
    connect( hide_zero_, SIGNAL(changed()), this, SLOT(update()) );
    check_interval_ = new LH_Qt_int(this,tr("Check interval (seconds)"),2,LH_FLAG_AUTORENDER);
}

void LH_MailcountText::input(lh_setup_item *item, int flags, int value)
{
    Q_UNUSED(item);
    Q_UNUSED(flags);
    Q_UNUSED(value);
}

int LH_MailcountText::notify(int n,void*p)
{
    Q_UNUSED(p);

    if( n & LH_NOTE_SECOND )
    {
        int delta = last_check_.secsTo( QTime::currentTime() );
        if( !last_check_.isValid() ||
            delta < 0 ||
            delta > check_interval_->value() )
            update();
    }

    return LH_NOTE_SECOND;
}

void LH_MailcountText::update()
{
    last_check_ = QTime::currentTime();
    count_ = getUnreadMailcount();
    if(setText((count_==0 && hide_zero_->value()? " " : QString::number(count_)))) this->requestRender();
}

int LH_MailcountText::getUnreadMailcount()
{
    int total = 0;

#ifdef Q_WS_WIN
    if( SHGetUnreadMailCountW )
    {
        HRESULT retv;
        SYSTEMTIME st;
        FILETIME ft;
        ULARGE_INTEGER mailtime;
        DWORD dwMail = 0;

        // Get unread mail count
        GetLocalTime( &st );
        SystemTimeToFileTime( &st, &ft );
        Q_ASSERT( sizeof(mailtime) == sizeof(ft) );
        memcpy( &mailtime, &ft, sizeof(mailtime) );
        mailtime.QuadPart -= (ULONGLONG)email_days_->value()*24*60*60*10000000; // subtract wanted number of days
        memcpy( &ft, &mailtime, sizeof(mailtime) );
        if( !email_addr_->value().isEmpty() )
        {
            retv = SHGetUnreadMailCountW( NULL, (LPCTSTR)(void*)email_addr_->value().utf16(), &dwMail, &ft, NULL, 0 );
        }
        else
            retv = SHGetUnreadMailCountW( NULL, NULL, &dwMail, &ft, NULL, 0 );
        if( retv != S_OK ) dwMail = 0;

        total += dwMail;
    }
#endif

    return total;
}

