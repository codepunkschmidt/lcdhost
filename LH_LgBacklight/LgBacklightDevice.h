#ifndef LGBACKLIGHTDEVICE_H
#define LGBACKLIGHTDEVICE_H

#include <QString>
#include <QByteArray>
#include <QColor>
#include "hidapi.h"

class LH_LgBacklight;

typedef struct LgBacklightReport_
{
    uchar hidprefix;
    uchar bReportId;
    uchar Red;
    uchar Green;
    uchar Blue;
} LgBacklightReport;

class LgBacklightDevice
{
    LH_LgBacklight *parent_;
    bool to_remove_;
    unsigned product_id_;
    QByteArray path_;
    QString name_;
    QColor color_;
    int backlightid_;

public:
    LgBacklightDevice( const struct hid_device_info *di, LH_LgBacklight *parent );

    QString name() const { return name_; }
    QColor color() const { return color_; }
    void setColor( QColor c );
    void setRemoval( bool b ) { to_remove_ = b; }
    bool removal() const { return to_remove_; }
    QByteArray path() const { return path_; }
    unsigned productId() const { return product_id_; }
};

#endif // LGBACKLIGHTDEVICE_H
