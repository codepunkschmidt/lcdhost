#include "LH_QtPlugin_MyLCDHostPlugin.h"

LH_QtPlugin_MyLCDHostPlugin::LH_QtPlugin_MyLCDHostPlugin(QObject *parent) :
    LH_QtPlugin(parent)
{
}

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>MyLCDHostPlugin</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl></versionurl>"
  "<author>My Name</author>"
  "<homepageurl>http://my.home.page</homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "What my plugin does"
  "</shortdesc>"
  "<longdesc>"
  "Detailed instructions and help"
  "</longdesc>"
"</lcdhostplugin>";

LH_PLUGIN(LH_QtPlugin_MyLCDHostPlugin)
