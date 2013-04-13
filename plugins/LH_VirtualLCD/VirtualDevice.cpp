#include "VirtualDevice.h"

VirtualDevice::VirtualDevice(LH_QtPlugin *drv) :
    LH_QtDevice(drv)
{
    setup_output_ = new LH_Qt_QImage(this,"Output",QImage(),LH_FLAG_HIDDEN);
}

const char* VirtualDevice::open()
{
    setup_output_->setFlag( LH_FLAG_HIDDEN, false );
    return NULL;
}

const char* VirtualDevice::close()
{
    setup_output_->setValue(QImage());
    setup_output_->setFlag( LH_FLAG_HIDDEN, true );
    return NULL;
}

const char* VirtualDevice::render_qimage(QImage *img)
{
    if( img )
    {
        if( depth() == 1 )
        {
            if( img->format() == QImage::Format_Mono ) setup_output_->setValue( *img );
            else setup_output_->setValue(  img->convertToFormat( QImage::Format_Mono ) );
        }
        else if( depth() == 32 )
        {
            if( img->format() == QImage::Format_ARGB32_Premultiplied ) setup_output_->setValue( *img );
            else setup_output_->setValue( img->convertToFormat( QImage::Format_ARGB32_Premultiplied ) );
        }
    }
    return NULL;
}
