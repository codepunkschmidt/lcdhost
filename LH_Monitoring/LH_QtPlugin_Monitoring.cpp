#include "LH_QtPlugin_Monitoring.h"

LH_PLUGIN(LH_QtPlugin_Monitoring)

const char *LH_QtPlugin_Monitoring::userInit()
{
    dataSources = new LH_MonitoringSources(this);
    dataSources->add(new LH_MonitoringSource_Aida64(this));
    dataSources->add(new LH_MonitoringSource_Afterburner(this));
    dataSources->add(new LH_MonitoringSource_ATITrayTools(this));
    dataSources->add(new LH_MonitoringSource_CoreTemp(this));
    dataSources->add(new LH_MonitoringSource_Fraps(this));
    dataSources->add(new LH_MonitoringSource_GPUZ(this));
    dataSources->add(new LH_MonitoringSource_HWiNFO(this));
    dataSources->add(new LH_MonitoringSource_HWMonitor(this));
    dataSources->add(new LH_MonitoringSource_Logitech(this));
    dataSources->add(new LH_MonitoringSource_RivaTuner(this));
    dataSources->add(new LH_MonitoringSource_SpeedFan(this));
    dataSources->userInit();

    QTimer::singleShot(5000, this, SLOT(connectEnabledStateChangeEvents()));
    return 0;
}

LH_Qt_bool* LH_QtPlugin_Monitoring::createUI_Element_Enabled(QString appName)
{
    enabled_.append( new LH_Qt_bool(this, "^"+appName, true, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDETITLE ) );
    return enabled_.last();
}

LH_Qt_QSlider* LH_QtPlugin_Monitoring::createUI_Element_Rate(QString appName)
{
    rate_helpers_.append( new LH_Qt_QString(this, appName + " update rate helper", "1 update per second", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_BLANKTITLE | LH_FLAG_READONLY | LH_FLAG_NOSAVE_LINK | LH_FLAG_NOSAVE_DATA | LH_FLAG_HIDDEN) );
    rates_.append( new LH_Qt_QSlider(this, appName + " update rate", 0 , -4, 4, LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_BLANKTITLE | LH_FLAG_HIDDEN ) );
    connect(rates_.last(), SIGNAL(changed()), this, SLOT(enabledFreqChanged()));
    return rates_.last();
}

LH_Qt_QString* LH_QtPlugin_Monitoring::createUI_Element_Divider(QString appName)
{
    if(dividers_.length()!=0)
        dividers_.last()->setHidden(false);
    dividers_.append( new LH_Qt_QString(this, "~divider_"+appName, "<hr />", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_NOSAVE_LINK | LH_FLAG_NOSAVE_DATA | LH_FLAG_HIDETITLE | LH_FLAG_HIDDEN, lh_type_string_html ) );
    return dividers_.last();
}

void LH_QtPlugin_Monitoring::enabledStateChanged()
{
    for(int i=0; i<enabled_.length(); i++)
    {
        rate_helpers_[i]->setVisible(enabled_[i]->value());
        rates_[i]->setVisible(enabled_[i]->value());
    }
    dataSources->refreshMonitoringOptions();
}

void LH_QtPlugin_Monitoring::enabledFreqChanged()
{
    for(int i=0; i<rates_.length(); i++)
    {
        int val = rates_[i]->value();
        if(val < 0)
            rate_helpers_[i]->setValue(QString("%1 seconds per update").arg(1-val));
        if(val == 0)
            rate_helpers_[i]->setValue("1 update per second");
        if(val > 0)
            rate_helpers_[i]->setValue(QString("%1 updates per second").arg(val+1));
    }
}

void LH_QtPlugin_Monitoring::connectEnabledStateChangeEvents()
{
    qDebug() << "LH_QtPlugin_Monitoring events connected";
    foreach(LH_Qt_bool *b, enabled_)
        connect(b, SIGNAL(changed()), this, SLOT(enabledStateChanged()));
    enabledStateChanged();
}






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
"<br />"
"<table>"


"<tr>"
"<td colspan='3'> <hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/aida64.png\"/></td>"
"<td width=\"100%\"><a href=\"http://http://www.aida64.com\">Aida64</a></td>"
"<td align='right'>(Commercial)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>Aida64 monitors a very large number of system statistics as well as the computer's voltages, fan speeds and temperatures in computers equipped with monitoring chips.<br /><br />"
"<i>N.B. This plugin extracts the data from Aida64 via Shared Memory (make sure this option is enabled in Preferences > Hardware Monitoring > External Applications).</i><hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/atitray.png\"/></td>"
"<td width=\"100%\"><a href=\"http://www.alcpu.com/ATITrayTools/\">ATITrayTools</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>ATITrayTools monitors various sensors in ATI graphics cards.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/CoreTemp.png\"/></td>"
"<td width=\"100%\"><a href=\"http://www.alcpu.com/CoreTemp/\">CoreTemp</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>CoreTemp monitors a computer's temperature sensors in computers equipped with monitoring chips, as well as things like CPU and FSB speeds, etc.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/fraps.png\"/></td>"
"<td width=\"100%\"><a href=\"http://www.fraps.com/\">Fraps</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>The main use of Fraps is to acquire the current \"Frames Per Second\". It is primarily of use within games, but can be used in windows (check the setting to monitor WDM in Fraps).<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/GPU-Z.png\"/></td>"
"<td width=\"100%\"><a href=\"http://www.techpowerup.com/gpuz/\">GPU-Z</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>GPU-Z monitors the various sensors in graphics cards.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/HWiNFO.png\"/></td>"
"<td width=\"100%\"><a href=\"http://www.hwinfo.com/\">HWiNFO</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>HWiNFO32 or HWiNFO64 monitors sensors on the motherboard, graphics cards, disk drives and more.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/HWMonitor.png\"/><img src=\":/images/apps/HwMonTray.png\"/></td>"
"<td width=\"100%\"><a href=\"http://www.hwinfo.com/\">HWMonitor</a> with <a href=\"http://blog.orbmu2k.de/tools/hardware-monitor-gadget-host\">HWMonTray</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>HWMonitor monitors graphics cards, motherboard sensors and disk drives. <i>However</i> it does not natively support sharing it's data. To solve this download HWMonTray aka \"HWMonitor GadgetHost\", and run HWMonTray which will then launch HWMonitor and make its data available to LCDHost.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/Logitech.png\"/></td>"
"<td width=\"100%\"><a href=\"http://forum.linkdata.se/lcdhost_plugins/windows_gadget_logitech_wireless_mouse_battery_monitoring_277.0.html/\">Logitech Mouse Battery Monitor</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>Not really an application, but an edited Wireless Mouse Battery monitoring gadget that exposes the battery level to LCDHost.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/MSIAfterburner.png\"/></td>"
"<td width=\"100%\"><a href=\"http://event.msi.com/vga/afterburner/\">MSI Afterburner</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>MSI Afterburner monitors a the various sensors in MSI graphics cards.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/RivaTuner.png\"/></td>"
"<td width=\"100%\"><a href=\"www.guru3d.com/index.php?page=rivatuner/\">RivaTuner</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>RivaTuner monitors a the various sensors in NVIDIA graphics cards.<hr/></td>"
"</tr>"

"<tr>"
"<td style=\"padding-right:2px\"><img src=\":/images/apps/speedfan.png\"/></td>"
"<td width=\"100%\"><a href=\"www.almico.com/speedfan.php\">SpeedFan</a></td>"
"<td align='right'>(Free)</td>"
"</tr>"
"<tr>"
"<td colspan='3'>SpeedFan monitors a computer's voltages, fan speeds and temperatures in computers equipped with monitoring chips.<hr/></td>"
"</tr>"


"</table>"
"</longdesc>"
"</lcdhostplugin>";

