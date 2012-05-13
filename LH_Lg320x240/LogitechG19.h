#ifndef LOGITECHG19_H
#define LOGITECHG19_H

#include <LH_QtDevice.h>
#include "libusb.h"

class LogitechG19 : public LH_QtDevice
{
    Q_OBJECT

    libusb_context *ctx_;
    libusb_device *usbdev_;
    libusb_device_handle *lcdhandle_;
    int menukeys_if_number_;
    int lcd_if_number_;
    uint8_t endpoint_in_;
    uint8_t endpoint_out_;
    volatile bool offline_;
    libusb_transfer * button_transfer_;
    int button_completed_;
    int last_buttons_;
    volatile quint16 new_buttons_;

public:
    static void g19_button_cb( struct libusb_transfer * transfer );
    LogitechG19( libusb_context *ctx, libusb_device *usbdev, libusb_device_descriptor *dd );
    ~LogitechG19();

    const char* render_argb32(int,int,const void*) { return NULL; }
    const char* render_mono(int,int,const void*) { return NULL; }
    const char* get_backlight(lh_device_backlight*) { return NULL; }
    const char* set_backlight(lh_device_backlight*) { return NULL; }

    void buttons( struct libusb_transfer * transfer = 0 );
    bool offline() const { return offline_; }

    const char* open();
    const char* close();
    const char* render_qimage(QImage *img);

};

#endif // LOGITECHG19_H
