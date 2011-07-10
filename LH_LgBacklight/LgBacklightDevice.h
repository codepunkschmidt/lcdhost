#ifndef LGBACKLIGHTDEVICE_H
#define LGBACKLIGHTDEVICE_H

#include <QString>
#include <QByteArray>

#include "../LH_Qt_QColor.h"
#include "hidapi.h"

class LH_LgBacklight;

class LgBacklightDevice
{
    LH_LgBacklight *parent_;
    bool to_remove_;
    unsigned product_id_;
    hid_device *hiddev_;
    QByteArray path_;
    LH_Qt_QColor *color_;
    QString name_;

public:
    LgBacklightDevice( const struct hid_device_info *di, LH_LgBacklight *parent );
    ~LgBacklightDevice();

    void setRemoval( bool b ) { to_remove_ = b; }
    bool removal() const { return to_remove_; }
    QByteArray path() const { return path_; }
    unsigned productId() const { return product_id_; }
};

#endif // LGBACKLIGHTDEVICE_H
