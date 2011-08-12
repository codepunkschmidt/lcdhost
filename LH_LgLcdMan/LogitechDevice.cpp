
#include <QCoreApplication>
#include "LogitechDevice.h"
#include "EventLgLcdButton.h"

#ifdef Q_WS_WIN
DWORD WINAPI LogitechDevice::LH_LogitechButtonCB(int device, DWORD dwButtons, const PVOID pContext)
#endif
#ifdef Q_WS_MAC
unsigned long LogitechDevice::LH_LogitechButtonCB(int device, unsigned long dwButtons, const void* pContext)
#endif
{
    LogitechDevice *ld = reinterpret_cast<LogitechDevice*>( (void*) pContext );
    Q_ASSERT( device != LGLCD_INVALID_DEVICE );
    if( ld )
        QCoreApplication::postEvent(
                ld,
                new EventLgLcdButton(dwButtons),
                Qt::HighEventPriority
                );
    return 1;
}

LogitechDevice::LogitechDevice( bool bw, LogitechManager *parent ) :
    LH_QtOutputDevice( bw ? "LH_LgLcdMan:BW" : "LH_LgLcdMan:QVGA",
          bw ? 160 : 320,
          bw ? 43 : 240,
          bw ? 1 : 32,
          false, parent ),
    bw_(bw),
    buttonState_(0),
    indev_(0)
{
    setTitle( bw_ ? "Logitech B/W LCD" : "Logitech QVGA LCD" );
    indev_ = new LH_QtInputDevice(ident(),lh_df_keyboard);
}

LogitechDevice::~LogitechDevice()
{
    if( indev_ ) delete indev_;
}

const char* LogitechDevice::render_qimage(QImage *p_image)
{
    if( p_image == NULL || p_image->isNull() )
        return 0;

    if( bw_ )
    {
        if( p_image->width() != 160 || p_image->height() != 43 )
            return "invalid image size";

        bm_.bmp_mono.hdr.Format = LGLCD_BMP_FORMAT_160x43x1;
        for( int y=0; y<43; ++y )
        {
            for( int x=0; x<160; ++x )
            {
                Q_ASSERT( (size_t)(y*160+x) < sizeof(bm_.bmp_mono.pixels) );
                bm_.bmp_mono.pixels[y*160 + x] = ( qGray( p_image->pixel(x,y) ) > 128) ? 0xFF : 0x00;
            }
        }
    }
    else
    {
        Q_ASSERT( p_image->numBytes() == sizeof( bm_.bmp_qvga32.pixels ) );
        bm_.bmp_qvga32.hdr.Format = LGLCD_BMP_FORMAT_QVGAx32;
        memcpy( bm_.bmp_qvga32.pixels,
    #ifdef Q_WS_MAC
                p_image->rgbSwapped().bits(),
    #else
                p_image->bits(),
    #endif
                sizeof( bm_.bmp_qvga32.pixels ) );
    }

    if( device() != LGLCD_INVALID_DEVICE && bm_.hdr.Format )
    {
        if( lgLcdUpdateBitmap( device(), &bm_.hdr, LGLCD_PRIORITY_NORMAL ) != ERROR_SUCCESS )
        {
            close();
        }
    }

    return 0;
}

void LogitechDevice::setButtonState( unsigned long button )
{
    if( buttonState_ != button )
    {
        for( unsigned long bit=0; bit<32; ++bit )
        {
            unsigned long mask = 1<<bit;
            if( (button&mask) != (buttonState_&mask) )
            {
                lh_input di;
                strncpy( di.ident, lh_dev()->obj.ident, sizeof(di.ident) );
                di.ident[sizeof(di.ident)-1] = 0;
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
        buttonState_ = button;
    }
    return;
}

void LogitechDevice::customEvent(QEvent *event)
{
    if( event->type() == EventLgLcdButton::type() )
    {
        EventLgLcdButton *be = static_cast<EventLgLcdButton*>(event);
        setButtonState( be->buttons );
        return;
    }
    LH_QtOutputDevice::customEvent(event);
}

