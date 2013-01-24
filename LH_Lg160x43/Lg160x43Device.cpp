
#include "Lg160x43Device.h"
#include "LH_HidDevice.h"

#include <QImage>

enum
{
        G15_KEY_READ_LENGTH = 9,/* largest input report size */
        G15_LCD_OFFSET = 32,	/* offset in output report where pixels start */
        G15_LCD_HEIGHT = 43,	/* LCD vertical height in pixels */
        G15_LCD_WIDTH = 160,	/* LCD horizontal width in pixels */
        G15_BUFFER_LEN = 0x03e0	/* total length of the HID output report */
};

Lg160x43Device::Lg160x43Device(LH_HidDevice *hi, int output_report_id, LH_QtPlugin *drv) :
    LH_QtDevice(drv),
    hd_(hi),
    output_report_id_((unsigned char)output_report_id)
{
    setDevid(hi->objectName());
    setName(hi->product_text());
    setSize(160,43);
    setDepth(1);
    setAutoselect(true);
    connect(hd_, SIGNAL(onlineChanged(bool)), this, SLOT(onlineChanged(bool)));
    arrive();
}

void Lg160x43Device::onlineChanged(bool b)
{
    if(hd_ && ! b)
    {
        hd_ = 0;
        leave();
        deleteLater();
    }
}

static void make_output_report(unsigned char *lcd_buffer, unsigned char const *data)
{
    unsigned int output_offset = G15_LCD_OFFSET;
    unsigned int base_offset = 0;

    memset( lcd_buffer, 0, G15_LCD_OFFSET );
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
    if(img && hd_ && hd_->online())
    {
        unsigned char buffer[G15_BUFFER_LEN];
        if( img->depth() == 1 )
            make_output_report( buffer, img->bits() );
        else
        {
            QImage tmp = img->convertToFormat(QImage::Format_Mono,Qt::ThresholdDither|Qt::NoOpaqueDetection);
            make_output_report( buffer, tmp.bits() );
        }
        buffer[0] = output_report_id_;
        hd_->write(QByteArray((char*)buffer, sizeof(buffer)));
    }
    return hd_->error();
}
