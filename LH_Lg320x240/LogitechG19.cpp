
#include <QDebug>
#include "LogitechG19.h"

LogitechG19::LogitechG19( const char *name, libusb_device *usbdev, libusb_device_descriptor *dd, QObject *parent ) : LH_QtDevice(name,parent)
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
    setId("G19");
    setName("Logitech G19 LCD (USB)");
    setSize(320,240);
    setDepth(16);
#ifdef Q_WS_WIN
    setAutoselect(false);
#else
    setAutoselect(true);
#endif

    setButtonName( 0x01, QObject::tr("App") );
    setButtonName( 0x02, QObject::tr("Cancel") );
    setButtonName( 0x04, QObject::tr("Menu") );
    setButtonName( 0x08, QObject::tr("Ok") );
    setButtonName( 0x10, QObject::tr("Right") );
    setButtonName( 0x20, QObject::tr("Left") );
    setButtonName( 0x40, QObject::tr("Down") );
    setButtonName( 0x80, QObject::tr("Up") );

    arrive();
}

LogitechG19::~LogitechG19()
{
    leave();
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

int LogitechG19::buttons()
{
    int usberr;
    int len = 0;
    quint16 u16;
    int but;

    if( offline_ ) return 0;
    if( !lcdhandle_ ) return 0;

    usberr = libusb_interrupt_transfer( lcdhandle_, endpoint_in_, (unsigned char*) &u16, sizeof(u16), &len, 10 );
    if( usberr == LIBUSB_ERROR_TIMEOUT && len == 0 ) return 0;
    if( usberr )
    {
        qDebug() << "LogitechG19::buttons() error" << QString::fromAscii(libusb_strerror((libusb_error)usberr) ) << "endpoint" << QString::number(endpoint_in_,16);
        return 0;
    }

    but = (u16 & 0x7FFF);
    if( but != last_buttons_ )
    {
        callback( lh_cb_button, (void*) but );
        last_buttons_ = but;
    }

    return but;
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




