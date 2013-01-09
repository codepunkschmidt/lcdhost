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

#include "LH_QtPlugin.h"
#include "LH_MonitoringSources.h"

#include "LH_MonitoringSource_Afterburner.h"
#include "LH_MonitoringSource_Aida64.h"
#include "LH_MonitoringSource_ATITrayTools.h"
#include "LH_MonitoringSource_CoreTemp.h"
#include "LH_MonitoringSource_Fraps.h"
#include "LH_MonitoringSource_GPUZ.h"
#include "LH_MonitoringSource_HWiNFO.h"
#include "LH_MonitoringSource_HWMonitor.h"
#include "LH_MonitoringSource_Logitech.h"
#include "LH_MonitoringSource_RivaTuner.h"
#include "LH_MonitoringSource_SpeedFan.h"

#include "QList"
#include "LH_Qt_QString.h"
#include "LH_Qt_bool.h"

class LH_QtPlugin_Monitoring : public LH_QtPlugin
{   
    Q_OBJECT

public:
    QList<LH_Qt_bool*> enabled_;
    QList<LH_Qt_QSlider*> rates_;
    QList<LH_Qt_QString*> rate_helpers_;
    QList<LH_Qt_QString*> dividers_;

    LH_Qt_bool* createUI_Element_Enabled(QString appName);

    LH_Qt_QSlider* createUI_Element_Rate(QString appName);

    LH_Qt_QString* createUI_Element_Divider(QString appName);

    const char *userInit();

    virtual void userTerm() { dataSources->userTerm(); }

    virtual int notify( int code, void *param )
    {
        if(dataSources)
            dataSources->rebuild();
        return LH_NOTE_SECOND;
    }

public slots:
    void enabledStateChanged();
    void enabledFreqChanged();
    void connectEnabledStateChangeEvents();

};

#endif // LH_QTPLUGIN_MONITORING_H
