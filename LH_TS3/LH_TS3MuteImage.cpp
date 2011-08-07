#include "LH_TS3MuteImage.h"

LH_PLUGIN_CLASS(LH_TS3MuteImage)

QImage* LH_TS3MuteImage::getPlaceholder()
{
    QImage* image = new QImage();
    QString source = setup_mute_source_->value();
    QString status = setup_mute_status_->value();
    image->load(QString(":/images/%1%2.png")
                .arg(status=="N/A"?"empty":(source=="Microphone"?"microphone":"sound"))
                .arg(status=="None"?"-disabled":(status=="Muted"?"-mute":""))
                );
    return image;
}

const char *LH_TS3MuteImage::userInit()
{
    if( const char *err = LH_QImage::userInit() ) return err;

    setup_show_placeholder_->setTitle("Use default images");

    setup_mute_source_ = new LH_Qt_QStringList(this, "Mute Source", QStringList() << "Speakers" << "Microphone", LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);

    setup_mute_status_ = new LH_Qt_QStringList(this, "Mute Status", QStringList() << "N/A" << "None" << "Muted" << "Active", LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_AUTORENDER);
    setup_mute_status_->setLink("Monitoring/3rdParty/TeamSpeak3/Speaker Status");
    setup_mute_status_->refreshData();

    connect(setup_mute_source_, SIGNAL(changed()), this, SLOT(changeSource()));

    return 0;
}

lh_class *LH_TS3MuteImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/TeamSpeak 3",
        "TS3MuteImage",
        "TS3 Mute Status (Image)",
        16,16
    };
    return &classInfo;
}

void LH_TS3MuteImage::changeSource()
{
    if(setup_mute_source_->value()=="Speakers")
        setup_mute_status_->setLink("Monitoring/3rdParty/TeamSpeak3/Speaker Status");
    if(setup_mute_source_->value()=="Microphone")
        setup_mute_status_->setLink("Monitoring/3rdParty/TeamSpeak3/Microphone Status");
    setup_mute_status_->refreshData();
}
