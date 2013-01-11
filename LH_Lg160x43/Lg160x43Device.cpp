
#include <QImage>
#include <QDebug>
#include "Lg160x43Device.h"
#include <errno.h>

enum
{
        G15_KEY_READ_LENGTH = 9,/* largest input report size */
        G15_LCD_OFFSET = 32,	/* offset in output report where pixels start */
        G15_LCD_HEIGHT = 43,	/* LCD vertical height in pixels */
        G15_LCD_WIDTH = 160,	/* LCD horizontal width in pixels */
        G15_BUFFER_LEN = 0x03e0	/* total length of the HID output report */
};

Lg160x43Device::Lg160x43Device( const struct hid_device_info *di, LH_QtPlugin *drv ) :
    LH_QtDevice(drv),
    to_remove_(false),
    offline_(true),
    product_id_(di->product_id),
    path_(di->path)
{
    char buf[64];
    qsnprintf( buf, sizeof(buf), "Lg160x43-%04x:%04x:%04x",
               di->vendor_id, di->product_id, di->release_number );
    QByteArray dev_id( buf );
    if( di->serial_number && *(di->serial_number) )
    {
        dev_id.append( '-' );
        dev_id.append( QString::fromWCharArray(di->serial_number).toLatin1() );
    }
    setDevid( dev_id );
    setName( QString::fromWCharArray(di->product_string) );
    setSize(160,43);
    setDepth(1);
    setAutoselect(true);
    arrive();
}

Lg160x43Device::~Lg160x43Device()
{
    close();
    leave();
}

const char* Lg160x43Device::open()
{
    if(hid_device *hd = lock())
    {
        unlock(hd);
        return NULL;
    }
    return offline_ ? NULL : error_.constData();
}

const char* Lg160x43Device::close()
{
    return NULL;
}

hid_device *Lg160x43Device::lock()
{
    error_.clear();
    errno = 0;
    if(hid_device *hd = hid_open_path( path_.constData() ))
    {
        offline_ = false;
        return hd;
    }
    error_.append("Failed to open ");
    error_.append(path_);
    if(errno)
    {
        error_.append(": ");
        error_.append(strerror(errno));
    }
    return 0;
}

void Lg160x43Device::unlock(hid_device *hd)
{
    if(hd)
        hid_close(hd);
}

static void make_output_report(unsigned char *lcd_buffer, unsigned char const *data)
{
    unsigned int output_offset = G15_LCD_OFFSET;
    unsigned int base_offset = 0;

    memset( lcd_buffer, 0, G15_LCD_OFFSET );
    lcd_buffer[0] = 0x03;

    for( int row=0; row<6; ++row )
    {
        for( int col=0; col<G15_LCD_WIDTH; ++col)
        {
            unsigned char b;
            unsigned int bit = col % 8;

            b = (((data[base_offset                        ] << bit) & 0x80) >> 7) |
                (((data[base_offset +  G15_LCD_WIDTH/8     ] << bit) & 0x80) >> 6) |
                (((data[base_offset + (G15_LCD_WIDTH/8 * 2)] << bit) & 0x80) >> 5);

            if( row < 5 )
            {
                b |= (((data[base_offset + (G15_LCD_WIDTH/8 * 3)] << bit) & 0x80) >> 4) |
                     (((data[base_offset + (G15_LCD_WIDTH/8 * 4)] << bit) & 0x80) >> 3) |
                     (((data[base_offset + (G15_LCD_WIDTH/8 * 5)] << bit) & 0x80) >> 2) |
                     (((data[base_offset + (G15_LCD_WIDTH/8 * 6)] << bit) & 0x80) >> 1) |
                     (((data[base_offset + (G15_LCD_WIDTH/8 * 7)] << bit) & 0x80) >> 0);
            }

#ifdef Q_OS_MAC
            lcd_buffer[ output_offset++ ] = b;
#else
            lcd_buffer[ output_offset++ ] = ~b;
#endif
            if( bit == 7 ) base_offset++;
        }
        base_offset += G15_LCD_WIDTH - (G15_LCD_WIDTH / 8);
    }
}

const char* Lg160x43Device::render_qimage(QImage *img)
{
    int retv;
    unsigned char buffer[ G15_BUFFER_LEN ];

    if( !img ) return NULL;
    if( offline_ ) return NULL;
    if(hid_device *hd = lock())
    {
        if( img->depth() == 1 ) make_output_report( buffer, img->bits() );
        else
        {
            QImage tmp = img->convertToFormat(QImage::Format_Mono,Qt::ThresholdDither|Qt::NoOpaqueDetection);
            make_output_report( buffer, tmp.bits() );
        }

        errno = 0;
        retv = hid_write( hd, buffer, sizeof(buffer) );
        if( retv != sizeof(buffer) )
        {
            error_.clear();
            error_.append(path_);
            error_.append(": hid_write() error: ");
            error_.append(strerror(errno));
        }
        unlock(hd);
    }

    if(error_.isEmpty())
        return NULL;
    offline_ = true;
    return error_.constData();
}

