
#include <QImage>
#include <QDebug>
#include "libusb.h"
#include "Lg320x240Device.h"

Lg320x240Device::Lg320x240Device( libusb_device *dev, libusb_device_descriptor *dd, LH_QtPlugin *drv )
    : LH_QtDevice(0,drv)
{
    Q_UNUSED( dd );
    Q_ASSERT( dev != NULL );
    Q_ASSERT( dd != NULL );

    dev_ = dev;
    dh_ = 0;
    to_remove_ = true;

#if 0
    if( !libusb_open( dev, &dh_ ) )
    {
        unsigned char buffer[255];

        if( libusb_get_string_descriptor_ascii( dh_, dd->iProduct, buffer, sizeof(buffer) ) > 0 )
            setName( QString::fromAscii((const char*)buffer) );
        else setName( "Lg320x240Device" );

        if( libusb_get_string_descriptor_ascii( dh_, dd->iSerialNumber, buffer, sizeof(buffer) ) > 0 )
            setId( QString::fromAscii((const char*)buffer) );
        else setId( QString("Lg320x240Device-%1").arg( (qptrdiff) dev_ ) );

        qDebug() << "adding" << objectName() << id();

        libusb_close( dh_ );
        dh_ = 0;

        to_remove_ = false;
        setSize(320,240);
        setDepth(16);
        setAutoselect(true);
        arrive();
    }
#endif
}

Lg320x240Device::~Lg320x240Device()
{
    close();
    if( dev_ )
    {
        libusb_unref_device( dev_ );
        dev_ = 0;
    }
}

const char* Lg320x240Device::open()
{
    if( !dev_ ) return "Lg320x240Device: no device pointer";
    if( !dh_ )
    {
        if( libusb_open( dev_, &dh_ ) ) return "failed to open device";
    }
    return NULL;
}

const char* Lg320x240Device::close()
{
    if( dh_ )
    {
        libusb_close( dh_ );
        dh_ = 0;
    }
    return NULL;
}


#define G19_BUFFER_LEN ((320*240*2)+512)

static void make_output_report(unsigned char *lcd_buffer, unsigned char const *data)
{
    static unsigned char header[16] = {0x10, 0x0F, 0x00, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x01, 0xEF, 0x00, 0x0F};
    lcd_buffer[0] = 0x02;
    memcpy( lcd_buffer, header, sizeof(header) );
    for( int i=16; i<256; ++i ) lcd_buffer[i] = i;
    for( int i=0; i<256; ++i ) lcd_buffer[i+256] = i;
    memcpy( lcd_buffer+512, data, 320*240*2 );
}

const char* Lg320x240Device::render_qimage(QImage *img)
{
    int retv = 0;
    unsigned char buffer[ G19_BUFFER_LEN ];

    if( !img ) return NULL;

    if( img->depth() == 16 ) make_output_report( buffer, img->bits() );
    else
    {
        QImage tmp = img->convertToFormat(QImage::Format_RGB16,Qt::NoOpaqueDetection);
        Q_ASSERT( 512+tmp.byteCount() == G19_BUFFER_LEN );
        make_output_report( buffer, tmp.bits() );
    }
    /*
        if len(data) != (320 * 240 * 2):
            raise ValueError("illegal frame size: " + str(len(data))
                    + " should be 320x240x2=" + str(320 * 240 * 2))
        frame = [0x10, 0x0F, 0x00, 0x58, 0x02, 0x00, 0x00, 0x00,
                 0x00, 0x00, 0x00, 0x3F, 0x01, 0xEF, 0x00, 0x0F]
        for i in range(16, 256):
            frame.append(i)
        for i in range(256):
            frame.append(i)
     */

    if( retv > 0 && retv != sizeof(buffer) )
    {
        // handle error
        return "hid_write() error";
    }
    return NULL;
}

