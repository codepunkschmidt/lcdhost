
#include <QDebug>
#include "LogitechG19.h"

LogitechG19::LogitechG19( libusb_device *usbdev, libusb_device_descriptor *dd, LH_QtObject *drv )
    : LH_QtDevice("LH_Lg320x240:G19",drv)
{
    usbdev_ = usbdev;
    lcdhandle_ = 0;
    endpoint_in_ = endpoint_out_ = 0;
    offline_ = false;

    for( int config_num=0; config_num<dd->bNumConfigurations; ++config_num )
    {
        struct libusb_config_descriptor *conf_desc = 0;
        const struct libusb_endpoint_descriptor *endpoint = 0;

        libusb_get_config_descriptor( usbdev_, config_num, &conf_desc );

        for(int i=0; i<conf_desc->bNumInterfaces && i<1; i++)
        {
            for (int j=0; j<conf_desc->interface[i].num_altsetting; j++)
            {
                printf("interface[%d].altsetting[%d]: num endpoints = %d\n",
                       i, j, conf_desc->interface[i].altsetting[j].bNumEndpoints);
                printf("   Class.SubClass.Protocol: %02X.%02X.%02X\n",
                       conf_desc->interface[i].altsetting[j].bInterfaceClass,
                       conf_desc->interface[i].altsetting[j].bInterfaceSubClass,
                       conf_desc->interface[i].altsetting[j].bInterfaceProtocol);

                for(int k=0; k<conf_desc->interface[i].altsetting[j].bNumEndpoints; k++)
                {
                    endpoint = &conf_desc->interface[i].altsetting[j].endpoint[k];
                    printf("       endpoint[%d].address: %02X\n", k, endpoint->bEndpointAddress);
                    // Use the last IN/OUT endpoints found as default for testing
                    if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN)
                    {
                        endpoint_in_ = endpoint->bEndpointAddress;
                    }
                    else
                    {
                        endpoint_out_ = endpoint->bEndpointAddress;
                    }
                    printf("           max packet size: %04X\n", endpoint->wMaxPacketSize);
                    printf("          polling interval: %02X\n", endpoint->bInterval);
                }
            }
        }
        if( conf_desc ) libusb_free_config_descriptor( conf_desc );
    }
    setObjectName("Logitech G19 LCD (USB)");
    setSize(320,240);
    setDepth(16);
#ifdef Q_WS_WIN
    setAutoselect(false);
#else
    setAutoselect(true);
#endif
}

LogitechG19::~LogitechG19()
{
}

#define ASSERT_USB(x) do { int retv_ = x; if( retv_ ) { qDebug() << #x << libusb_strerror((libusb_error)retv_); return libusb_strerror((libusb_error) retv_); } } while(0)

const char* LogitechG19::open()
{
    Q_ASSERT( lcdhandle_ == 0 );

    ASSERT_USB( libusb_open(usbdev_, &lcdhandle_) );
    ASSERT_USB( libusb_set_configuration( lcdhandle_, 1 ) );
    ASSERT_USB( libusb_claim_interface( lcdhandle_, lcd_if_number_ ) );
    ASSERT_USB( libusb_claim_interface( lcdhandle_, menukeys_if_number_ ) );

    // libusb_detach_kernel_driver( usbhandle_, menukeys_if_number_ );
    return NULL;
}

const char* LogitechG19::close()
{
    if( lcdhandle_ ) libusb_close( lcdhandle_ ); lcdhandle_ = 0;
    return NULL;
}

const char* LogitechG19::input_name(const char *devid, int n)
{
    Q_UNUSED(devid);

    switch(n)
    {
    case 0: return "Logitech G19";
    case 0x01: return "App";
    case 0x02: return "Cancel";
    case 0x04: return "Menu";
    case 0x08: return "Ok";
    case 0x10: return "Right";
    case 0x20: return "Left";
    case 0x40: return "Down";
    case 0x80: return "Up";
    }
    return LH_QtObject::input_name(devid,n);
}

int LogitechG19::buttons()
{
    int usberr;
    int len = 0;
    quint16 u16;
    int button;

    if( offline_ ) return 0;
    if( !lcdhandle_ ) return 0;

    usberr = libusb_interrupt_transfer( lcdhandle_, endpoint_in_, (unsigned char*) &u16, sizeof(u16), &len, 10 );
    if( usberr == LIBUSB_ERROR_TIMEOUT && len == 0 ) return 0;
    if( usberr )
    {
        qDebug() << "LogitechG19::buttons() error" << QString::fromAscii(libusb_strerror((libusb_error)usberr) ) << "endpoint" << QString::number(endpoint_in_,16);
        return 0;
    }

    button = (u16 & 0x7FFF);
    if( button != last_buttons_ )
    {
        // send delta
        for( int bit=0; bit<16; ++bit )
        {
            int mask = 1<<bit;
            if( (button&mask) != (last_buttons_&mask) )
            {
                lh_input di;
                strcpy( di.devid, lh_dev()->devid );
                di.item = bit;
                di.flags = lh_df_button;
                if( button & mask )
                {
                    di.flags |= lh_df_down;
                    di.value = 0xFFFF;
                }
                else
                {
                    di.flags |= lh_df_up;
                    di.value = 0x0;
                }
                callback( lh_cb_input, (void*) &di );
            }
        }
        last_buttons_ = button;
    }

    return button;
}

#define G19_BUFFER_LEN ((320*240*2)+512)

const char* LogitechG19::render_qimage(QImage *img)
{
    static unsigned char header[16] = {0x10, 0x0F, 0x00, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x01, 0xEF, 0x00, 0x0F};
    unsigned char lcd_buffer[ G19_BUFFER_LEN ], *p;
    int len = 0;
    int usberr = 0;

    if( offline_ ) return NULL;
    if( !lcdhandle_ ) return "G19 not opened";
    if( !img ) return NULL;

    lcd_buffer[0] = 0x02;
    memcpy( lcd_buffer, header, sizeof(header) );
    for( int i=16; i<256; ++i ) lcd_buffer[i] = i;
    for( int i=0; i<256; ++i ) lcd_buffer[i+256] = i;
    p = lcd_buffer + 512;
    for( int x = 0; x < 320; ++x )
    {
        for( int y = 0; y < 240; ++y )
        {
            QRgb pix = img->pixel(x,y);
            uchar g = (qGreen(pix) >> 2);
            *p++ = (g<<5) | (qBlue(pix) >> 3);
            *p++ = (qRed(pix)&0xF8) | (g>>3);
        }
    }

    usberr = libusb_bulk_transfer(
            lcdhandle_,
            endpoint_out_,
            lcd_buffer,
            sizeof(lcd_buffer),
            &len,
            1000
            );

    if( usberr || len != sizeof(lcd_buffer) )
    {
        offline_ = true;
        qDebug() << "LogitechG19::render_qimage():" << libusb_strerror((libusb_error)usberr);
        return libusb_strerror((libusb_error)usberr);
    }

    return NULL;
}




