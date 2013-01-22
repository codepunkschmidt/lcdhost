/**
  \file     LH_Lg160x43.cpp
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

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#include "LH_Lg160x43.h"
#include "Lg160x43Device.h"
#include "LH_HidDevice.h"

LH_PLUGIN(LH_Lg160x43)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Lg160x43</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Logitech 160x43 LCD via HID"
  "</shortdesc>"
  "<longdesc>"
    "HID-level driver for Logitech 160x43 LCD displays, such as G13 or G15.<br/>"
    "Note that to use this driver you may need to uninstall existing drivers for these devices."
  "</longdesc>"
"</lcdhostplugin>";

const char *LH_Lg160x43::userInit()
{
#ifdef Q_OS_WIN32
    // make sure neither LCDMon.exe nor LCORE.EXE is running on Windows
    if( FindWindowA( "Logitech LCD Monitor Window", "LCDMon" ) ||
        FindWindowA( "QWidget", "LCore" ) )
        return "Logitech drivers are loaded";
#endif
    LH_HidDevice::subscribe(this, SLOT(onlineChanged(LH_HidDevice*,bool)));
    return 0;
}

void LH_Lg160x43::onlineChanged(LH_HidDevice *hd, bool b)
{
    if(b && hd->vendor_id() == 0x046d)
    {
        switch(hd->product_id())
        {
        case 0xC222: /* G15 */
        case 0x0A07: /* Z10 */
        case 0xC227: /* G15v2 */
        case 0xC21C: /* G13 */
        case 0xC22D: /* G510 without audio */
        case 0xC22E: /* G510 with audio */
            new Lg160x43Device(hd, this);
            break;
        }
    }
}
