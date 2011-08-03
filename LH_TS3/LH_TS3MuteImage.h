#ifndef LH_TS3MUTEIMAGE_H
#define LH_TS3MUTEIMAGE_H

#include "../LH_Monitoring/LH_QImage/LH_QImage.h"
#include "../LH_Qt_QStringList.h"

class LH_TS3MuteImage : public LH_QImage
{
    Q_OBJECT

protected:
    LH_Qt_QStringList *setup_mute_source_;
    LH_Qt_QStringList *setup_mute_status_;

public:
    LH_TS3MuteImage();
    const char *userInit();

public slots:
    void changeSource();
};

#endif // LH_TS3MUTEIMAGE_H
