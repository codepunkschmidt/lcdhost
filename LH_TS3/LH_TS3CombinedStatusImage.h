#ifndef LH_TS3COMBINEDSTATUSIMAGE_H
#define LH_TS3COMBINEDSTATUSIMAGE_H

#include "LH_QImage.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QStringList.h"
#include <QDebug>

class LH_TS3CombinedStatusImage : public LH_QImage
{
    Q_OBJECT

protected:
    LH_Qt_QStringList *setup_connection_status_;
    LH_Qt_QString *setup_talking_;
    LH_Qt_bool *setup_talking_me_;
    LH_Qt_QString *setup_nickname_;
    LH_Qt_QStringList *setup_combined_status_;
    LH_Qt_QStringList *setup_mute_status_spk_;
    LH_Qt_QStringList *setup_mute_status_mic_;

    QImage* getPlaceholder();

public:
    const char *userInit();
    static lh_class *classInfo();

public slots:
    void updateCombinedStatus();
};

#endif // LH_TS3COMBINEDSTATUSIMAGE_H
