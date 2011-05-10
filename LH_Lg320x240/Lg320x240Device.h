#ifndef LG320x240DEVICE_H
#define LG320x240DEVICE_H

#include "../LH_QtDevice.h"
#include "libusb.h"

class Lg320x240Device : public LH_QtDevice
{
    Q_OBJECT

    bool to_remove_;
    unsigned product_id_;
    libusb_device *dev_;
    libusb_device_handle *dh_;

public:
    Lg320x240Device( libusb_device *dev, libusb_device_descriptor *dd, LH_QtPlugin *drv = 0 );
    ~Lg320x240Device();

    void setRemoval( bool b ) { to_remove_ = b; }
    bool removal() const { return to_remove_; }

    unsigned productId() const { return product_id_; }
    libusb_device *libusb_dev() const { return dev_; }


    const char* open();

    const char* render_argb32(int,int,const void*) { return NULL; }
    const char* render_mono(int,int,const void*) { return NULL; }
    int buttons() { return 0; }
    const char* get_backlight(lh_device_backlight*) { return NULL; }
    const char* set_backlight(lh_device_backlight*) { return NULL; }

    const char* close();

    const char* render_qimage(QImage *img);
};

#endif // LG320x240DEVICE_H
