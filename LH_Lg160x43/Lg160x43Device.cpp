
#include <QImage>
#include <QDebug>
#include "Lg160x43Device.h"

enum
{
        G15_KEY_READ_LENGTH = 9,/* largest input report size */
        G15_LCD_OFFSET = 32,	/* offset in output report where pixels start */
        G15_LCD_HEIGHT = 43,	/* LCD vertical height in pixels */
        G15_LCD_WIDTH = 160,	/* LCD horizontal width in pixels */
        G15_BUFFER_LEN = 0x03e0	/* total length of the HID output report */
};

Lg160x43Device::Lg160x43Device( const struct hid_device_info *di, LH_QtPlugin *drv ) : LH_QtDevice(NULL,drv)
{
    to_remove_ = false;
    product_id_ = di->product_id;
    hiddev_ = 0;
    path_ = di->path;
    setId(di->path);
    setName( QString::fromWCharArray(di->product_string) );
    setSize(160,43);
    setDepth(1);
    setAutoselect(false);
    arrive();
}

Lg160x43Device::~Lg160x43Device()
{
    close();
    leave();
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

            lcd_buffer[ output_offset++ ] = ~b;
            if( bit == 7 ) base_offset++;
        }
        base_offset += G15_LCD_WIDTH - (G15_LCD_WIDTH / 8);
    }
}

const char* Lg160x43Device::render_qimage(QImage *img)
{
    int retv;
    unsigned char buffer[ G15_BUFFER_LEN+1 ];

    if( !img ) return NULL;
    if( !hiddev_ ) return "Device not open";

    if( img->depth() == 1 ) make_output_report( buffer+1, img->bits() );
    else
    {
        QImage tmp = img->convertToFormat(QImage::Format_Mono,Qt::ThresholdDither|Qt::NoOpaqueDetection);
        make_output_report( buffer+1, tmp.bits() );
    }

    buffer[0] = 0;

    retv = hid_write( hiddev_, buffer, sizeof(buffer) );
    if( retv != sizeof(buffer) )
    {
        qDebug() << "Lg160x43Device: hid write error";
        // handle error
        return "hid_write() error";
    }
    return NULL;
}

