/**
  \file     LH_VirtualLCD.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <QFile>
#include <QDebug>

#include "../LH_QtOutputDevice.h"
#include "LH_VirtualLCD.h"
#include "../LH_Qt_QImage.h"

LH_PLUGIN(LH_QtPlugin_VirtualLCD)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Virtual LCD</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Virtual LCD driver"
  "</shortdesc>"
  "<longdesc>"
  "This driver provides two software emulated LCD's, one QVGA and one monochrome 160x43."
  "</longdesc>"
"</lcdhostplugin>";

class VirtualLCD : public LH_QtOutputDevice
{
protected:
    LH_Qt_QImage *setup_output_;

public:
    VirtualLCD( const char *devid, int w, int h, int d, bool noauto ) :
        LH_QtOutputDevice(devid,w,h,d,noauto) {}

    const char *userInit()
    {
        if( const char *err = LH_QtOutputDevice::userInit() ) return err;
        setup_output_ = new LH_Qt_QImage(this,"Output",QImage(),LH_FLAG_HIDDEN);
        return 0;
    }

    const char* open()
    {
        setup_output_->setFlag( LH_FLAG_HIDDEN, false );
        return NULL;
    }

    const char* render_argb32(int,int,const void*) { return NULL; }
    const char* render_mono(int,int,const void*) { return NULL; }
    int buttons() { return 0; }
    const char* get_backlight(lh_device_backlight*) { return NULL; }
    const char* set_backlight(lh_device_backlight*) { return NULL; }

    const char* close()
    {
        setup_output_->setValue(QImage());
        setup_output_->setFlag( LH_FLAG_HIDDEN, true );
        return NULL;
    }

    const char* render_qimage(QImage *img)
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
};

const char *LH_QtPlugin_VirtualLCD::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;
    new VirtualLCD( "LH_VirtualLCD:320x240x32", 320, 240, 32, true );
    new VirtualLCD( "LH_VirtualLCD:160x43x1", 160, 43, 1, true );
    return 0;
}
