#include "LH_TS3MuteImage.h"

LH_TS3MuteImage::LH_TS3MuteImage()
{
}

const char *LH_TS3MuteImage::userInit()
{
    if( const char *err = LH_QImage::userInit() ) return err;

    setup_mute_source_ = new LH_Qt_QStringList(this, "Mute Source", QStringList() << "Speakers" << "Microphone", LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_AUTORENDER);

    setup_mute_status_ = new LH_Qt_QStringList(this, "Mute Status", QStringList() << "N/A" << "None" << "Muted" << "Active", LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE | LH_FLAG_READONLY | LH_FLAG_AUTORENDER);
    setup_mute_status_->item()->states |= LH_STATE_SOURCE;
    setup_mute_status_->setLink("Monitoring/3rdParty/TeamSpeak3/Speaker Status");
    setup_mute_status_->refreshData();

    connect(setup_mute_source_, SIGNAL(changed()), this, SLOT(changeSource()));

    return 0;
}

void LH_TS3MuteImage::changeSource()
{

}
