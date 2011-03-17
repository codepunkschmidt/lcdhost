/**
  \file     LH_QtPlugin_Text.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009 Johan Lindh

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#ifndef LH_QTPLUGIN_MONITORING_H
#define LH_QTPLUGIN_MONITORING_H

#include "../LH_QtPlugin.h"

class LH_QtPlugin_Monitoring : public LH_QtPlugin
{
public:
    const char * lh_name() { return "Monitoring (v1.03)"; }
    const char * lh_shortdesc() { return "Connects to a variety of 3rd party applications and reports statistics such as Temperatures, Fan Speeds, Voltages, etc."; }
    const char * lh_author() { return "Andy \"Triscopic\" Bridges"; }
    const char * lh_homepage() { return "<a href=\"http://www.codeleap.co.uk\">CodeLeap</a>"; }
    const lh_buildinfo * lh_get_buildinfo() {
        static lh_buildinfo buildinfo =
        {
            "1.03",
            REVISION,
            "http://codeleap.co.uk/lcdhost/lh_monitoring/ver.txt",
            "http://codeleap.co.uk/lcdhost/lh_monitoring/LH_Monitoring.dll"
        };
        return &buildinfo;
    }
    const char * lh_longdesc()
    {
        return "This plugin provides connections through to several 3rd Party applications, from which it can then extract information whenever they are running: "
               "<br /><br />"
               "<table>"

               "<tr>"
               "<td style='padding-right:8px;'><a href=\"http://event.msi.com/vga/afterburner/\">MSI Afterburner</a> (free)</td>"
               "<td>MSI Afterburner monitors a the various sensors in MSI graphics cards.<br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://http://www.aida64.com\">Aida64</a> (commercial)</td>"
               "<td>Aida64 monitors a very large number of system statistics as well as the computer's voltages, fan speeds and temperatures in computers equipped with monitoring chips.<br /><br />"
               "<i>N.B. This plugin extracts the data from Aida64 via Shared Memory (make sure this option is enabled in Preferences > Hardware Monitoring > External Applications).</i><br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://www.alcpu.com/ATITrayTools/\">ATITrayTools</a> (free)</td>"
               "<td>ATITrayTools monitors various sensors in ATI graphics gards.<br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://www.alcpu.com/CoreTemp/\">CoreTemp</a> (free)</td>"
               "<td>CoreTemp monitors a computer's temperature sensors in computers equipped with monitoring chips, as well as things like CPU and FSB speeds, etc.<br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://www.fraps.com/\">Fraps</a> (free)</td>"
               "<td>The main use of Fraps is to acquire the current \"Frames Per Second\". It is primarily of use within games, but can be used in windows (check the setting to monitor WDM in Fraps).<br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://www.techpowerup.com/gpuz/\">GPU-Z</a> (free)</td>"
               "<td>GPU-Z monitors the various sensors in graphics cards.<br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://www.guru3d.com/index.php?page=rivatuner/\">RivaTuner</a> (free)</td>"
               "<td>RivaTuner monitors a the various sensors in NVIDIA graphics cards.<br /></td>"
               "</tr>"

               "<tr>"
               "<td><a href=\"http://www.almico.com/speedfan.php\">SpeedFan</a> (free)</td>"
               "<td>SpeedFan monitors a computer's voltages, fan speeds and temperatures in computers equipped with monitoring chips.<br /></td>"
               "</tr>"

               "</table>"
               ;
    }
};

extern LH_QtPlugin_Monitoring thePlugin;

#endif // LH_QTPLUGIN_MONITORING_H
