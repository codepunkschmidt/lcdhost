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
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_MailcountText::LH_MailcountText()
    : LH_Text()
    , email_count_(0)
    , hide_limit_(0)
{
}

const char *LH_MailcountText::userInit()
{
    if (const char* msg = LH_Text::userInit()) return msg;
    setup_text_->setName( "Unread mail count" );
    setup_text_->setValue("0");
    setup_text_->setFlag( LH_FLAG_READONLY, true );
    setup_text_->setFlag( LH_FLAG_NOSAVE_DATA, true );

    email_count_ = new LH_Qt_int(this,tr("Mail count"),0,LH_FLAG_NOSAVE_DATA|LH_FLAG_NOSAVE_LINK|LH_FLAG_NOSOURCE);
    email_count_->setSubscribePath("/system/Mail count");
    connect( email_count_, SIGNAL(changed()), this, SLOT(update()) );

    hide_limit_ = new LH_Qt_int(this,tr("Hide when less than"),0,LH_FLAG_AUTORENDER);
    connect( hide_limit_, SIGNAL(changed()), this, SLOT(update()) );
    return 0;
}

void LH_MailcountText::update()
{
    QString wanttext = " ";
    int count = email_count_->value();
    if( count >= hide_limit_->value() ) wanttext = QString::number(count);
    if( setText( wanttext ) )
        requestRender();
}
