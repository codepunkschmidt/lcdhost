#ifndef LG160X43DEVICE_H
#define LG160X43DEVICE_H

#include "LH_QtDevice.h"
#include "hidapi.h"

class Lg160x43Device : public LH_QtDevice
{
    Q_OBJECT

    bool to_remove_;
    bool offline_;
    unsigned product_id_;
    QByteArray path_;
    QByteArray error_;

public:
    Lg160x43Device( const struct hid_device_info *di, LH_QtPlugin *drv );
    ~Lg160x43Device();

    void setRemoval( bool b ) { to_remove_ = b; }
    bool removal() const { return to_remove_; }
    bool offline() const { return offline_; }
    QByteArray path() const { return path_; }

    unsigned productId() const { return product_id_; }

    const char* open();
    const char* render_argb32(int,int,const void*) { return NULL; }
    const char* render_mono(int,int,const void*) { return NULL; }
    int buttons() { return 0; }
    const char* get_backlight(lh_device_backlight*) { return NULL; }
    const char* set_backlight(lh_device_backlight*) { return NULL; }
    const char* close();

    const char* render_qimage(QImage *img);

protected:
    hid_device *lock();
    void unlock(hid_device *);
};

#endif // LG160X43DEVICE_H
