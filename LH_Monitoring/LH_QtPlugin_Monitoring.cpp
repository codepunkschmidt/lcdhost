#include "LH_QtPlugin_Monitoring.h"

LH_PLUGIN(LH_QtPlugin_Monitoring)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Monitoring</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" STRINGIZE(VERSION) "\nr" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Andy \"Triscopic\" Bridges</author>"
  "<homepageurl><a href=\"http://www.codeleap.co.uk\">CodeLeap</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Connects to a variety of 3rd party applications and reports statistics such as Temperatures, Fan Speeds, Voltages, etc."
  "</shortdesc>"
  "<longdesc>"
"This plugin provides connections through to several 3rd Party applications, from which it can then extract information whenever they are running: "
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
"</longdesc>"
"</lcdhostplugin>";
