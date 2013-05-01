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

#include "LH_QtDevice.h"
#include "LH_VirtualLCD.h"
#include "LH_Qt_QImage.h"
#include "VirtualDevice.h"

LH_PLUGIN(LH_VirtualLCD)

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

class VirtualQVGA : public VirtualDevice
{
public:
    VirtualQVGA( LH_QtPlugin *drv ) : VirtualDevice( drv )
    {
        setDevid("320x240x32");
        setName("Virtual 320x240x32 device");
        setSize(320,240);
        setDepth(32);
        setAutoselect(false);
        arrive();
    }
};

class VirtualBW : public VirtualDevice
{
public:
    VirtualBW( LH_QtPlugin *drv ) : VirtualDevice( drv )
    {
        setDevid("160x43x1");
        setName("Virtual 160x43x1 device");
        setSize(160,43);
        setDepth(1);
        setAutoselect(false);
        arrive();
    }
};

const char *LH_VirtualLCD::userInit()
{
    new VirtualQVGA(this);
    new VirtualBW(this);
    return 0;
}
