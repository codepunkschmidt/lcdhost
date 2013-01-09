
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include "LogitechG19.h"

void LIBUSB_CALL LogitechG19::g19_button_cb( struct libusb_transfer * transfer )
{
    if( transfer && transfer->user_data )
    {
        LogitechG19 * g19 = (LogitechG19*) transfer->user_data;
        Q_ASSERT( g19->button_transfer_ == transfer );
        g19->button_completed_ = 1;
        if( ! g19->offline() )
        {
            if( transfer->status == LIBUSB_TRANSFER_COMPLETED || transfer->status == LIBUSB_TRANSFER_TIMED_OUT )
            {
                g19->buttons();
                libusb_submit_transfer( transfer );
                g19->button_completed_ = 0;
            }
        }
    }

    return;
}

LogitechG19::LogitechG19( libusb_context *ctx, libusb_device *usbdev, libusb_device_descriptor *dd ) :
    LH_QtDevice( 0 ),
    ctx_(ctx),
    usbdev_(usbdev),
    lcdhandle_(0),
    menukeys_if_number_(0),
    lcd_if_number_(0),
    endpoint_in_(0),
    endpoint_out_(0),
    offline_(false),
    button_transfer_(0),
    button_completed_(1),
    last_buttons_(0),
    new_buttons_(0)
{
    for( int config_num=0; config_num<dd->bNumConfigurations; ++config_num )
    {
        struct libusb_config_descriptor *conf_desc = 0;
        const struct libusb_endpoint_descriptor *endpoint = 0;

        libusb_get_config_descriptor( usbdev_, config_num, &conf_desc );

        for(int i=0; i<conf_desc->bNumInterfaces && i<1; i++)
        {
            for (int j=0; j<conf_desc->interface[i].num_altsetting; j++)
            {
#if 0
                printf("interface[%d].altsetting[%d]: num endpoints = %d\n",
                       i, j, conf_desc->interface[i].altsetting[j].bNumEndpoints);
                printf("   Class.SubClass.Protocol: %02X.%02X.%02X\n",
                       conf_desc->interface[i].altsetting[j].bInterfaceClass,
                       conf_desc->interface[i].altsetting[j].bInterfaceSubClass,
                       conf_desc->interface[i].altsetting[j].bInterfaceProtocol);
#endif
                for(int k=0; k<conf_desc->interface[i].altsetting[j].bNumEndpoints; k++)
                {
                    endpoint = & conf_desc->interface[i].altsetting[j].endpoint[k];
#if 0
                    printf("       endpoint[%d].address: %02X\n", k, endpoint->bEndpointAddress);
                    printf("           max packet size: %04X\n", endpoint->wMaxPacketSize);
                    printf("          polling interval: %02X\n", endpoint->bInterval);
#endif
                    if( endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN )
                        endpoint_in_ = endpoint->bEndpointAddress;
                    else
                        endpoint_out_ = endpoint->bEndpointAddress;
                }
            }
        }
        if( conf_desc ) libusb_free_config_descriptor( conf_desc );
    }
    setDevid("Lg320x240-G19");
    setName("Logitech G19 LCD (USB)");
    setSize(320,240);
    setDepth(16);
#ifdef Q_OS_WIN32
    setAutoselect(false);
#else
    setAutoselect(true);
#endif
}

LogitechG19::~LogitechG19()
{
    close();
}

#define ASSERT_USB(x) if( retv == 0 ) { retv = x; if( retv != 0 ) { qDebug() << #x << libusb_error_name( retv ); } }

const char* LogitechG19::open()
{
    int retv = 0;
    const char * errstr = 0;

    Q_ASSERT( button_transfer_ == 0 );
    Q_ASSERT( button_completed_ == 1 );
    Q_ASSERT( lcdhandle_ == 0 );

    last_buttons_ = new_buttons_ = 0;
    button_transfer_ = 0;
    button_completed_ = 1;

    ASSERT_USB( libusb_open(usbdev_, &lcdhandle_) );
    if( lcdhandle_ )
    {
        ASSERT_USB( libusb_set_configuration( lcdhandle_, 1 ) );
        ASSERT_USB( libusb_claim_interface( lcdhandle_, lcd_if_number_ ) );
        ASSERT_USB( libusb_claim_interface( lcdhandle_, menukeys_if_number_ ) );
        offline_ = false;
        button_transfer_ = libusb_alloc_transfer( 0 );
        libusb_fill_interrupt_transfer(
                    button_transfer_,
                    lcdhandle_,
                    endpoint_in_,
                    (unsigned char*) &new_buttons_,
                    sizeof(new_buttons_),
                    g19_button_cb,
                    this, 60000
                    );
        libusb_submit_transfer( button_transfer_ );
        button_completed_ = 0;
        return NULL;
    }

    offline_ = true;
    if( retv ) errstr = libusb_error_name( retv );
    if( errstr == 0 || !*errstr ) errstr = "libusb_open() returned NULL handle";
    qCritical( "LogitechG19::open() failed: '%s'", errstr );
    return errstr;
}

const char* LogitechG19::close()
{
    offline_ = true;
    if( button_transfer_ )
    {
        libusb_cancel_transfer( button_transfer_ );
        while( ! button_completed_ )
            libusb_handle_events_completed( ctx_, & button_completed_ );
        libusb_free_transfer( button_transfer_ );
        button_transfer_ = 0;
    }
    if( lcdhandle_ )
    {
        libusb_close( lcdhandle_ );
        lcdhandle_ = 0;
    }
    return NULL;
}

void LogitechG19::buttons( libusb_transfer * )
{
    if( offline_ || lcdhandle_ == 0 )
        return;

    new_buttons_ &= 0x7FFF;
    if( new_buttons_ != last_buttons_ )
    {
        // send delta
        for( int bit=0; bit<16; ++bit )
        {
            int mask = 1<<bit;
            if( (new_buttons_ & mask) != (last_buttons_ & mask) )
            {
                lh_device_input di;
                di.devid = lh_dev()->devid;
                switch( mask )
                {
                case 0x01: di.control = "App"; break;
                case 0x02: di.control = "Cancel"; break;
                case 0x04: di.control = "Menu"; break;
                case 0x08: di.control = "Ok"; break;
                case 0x10: di.control = "Right"; break;
                case 0x20: di.control = "Left"; break;
                case 0x40: di.control = "Down"; break;
                case 0x80: di.control = "Up"; break;
                default: di.control = "Unknown"; break;
                }
                di.item = bit;
                di.flags = lh_df_button;
                if( new_buttons_ & mask )
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
        last_buttons_ = new_buttons_;
    }

    return;
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
        qDebug() << "LogitechG19::render_qimage():" << libusb_error_name((libusb_error)usberr);
        return libusb_error_name((libusb_error)usberr);
    }

    return NULL;
}




