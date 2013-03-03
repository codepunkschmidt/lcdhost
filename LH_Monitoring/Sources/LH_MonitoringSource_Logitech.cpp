#include "LH_MonitoringSource_Logitech.h"
#include <QRegExp>
#include <QSettings>
//#include <algorithm>

LH_MonitoringSource_Logitech::LH_MonitoringSource_Logitech(LH_QtObject *parent): LH_MonitoringSource(parent, "Logitech Monitoring Gadget") {
    batteryStates_.append(QStringList()<<"Unknown");
    batteryStates_.append(QStringList()<<"Unknown"<<"Dummy: Nothing has one level");
    batteryStates_.append(QStringList()<<"Unknown"<<"Low"<<"High");
    batteryStates_.append(QStringList()<<"Unknown"<<"Low"<<"Medium"<<"High");
    batteryStates_.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium-High"<<"High");
    batteryStates_.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium-High"<<"High");
    batteryStates_.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium"<<"Medium-High"<<"High");
    batteryStates_.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium"<<"Medium"<<"Medium-High"<<"High");

    pcnt_ = SensorDefinition("%");
}


bool LH_MonitoringSource_Logitech::doUpdate()
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\CodeLeap\\LogitechMonitoring", QSettings::NativeFormat);

    int MonitoringInstalled = -1;
    int MouseBatteryLevel = -1;
    int MouseBatteryRange = -1;
    QString MouseBatteryState = "N/A";

    if(reg.contains("MonitoringInstalled"))
        MonitoringInstalled = reg.value("MonitoringInstalled").toInt();
    if(reg.contains("MouseBatteryLevel"))
        MouseBatteryLevel = reg.value("MouseBatteryLevel").toInt();
    if(reg.contains("MouseBatteryRange"))
        MouseBatteryRange = reg.value("MouseBatteryRange").toInt();
    if(reg.contains("MouseBatteryState"))
        MouseBatteryState = reg.value("MouseBatteryState").toString();

    setDataAvailable(
                MonitoringInstalled == 1 &&
                MouseBatteryRange != -1 &&
                MouseBatteryLevel != -1);
    if(!dataAvailable())
        return false;
    else
    {
        bool successfulUpdate = (reg.contains("MouseBatteryLevel")|| reg.contains("MouseBatteryRange") || reg.contains("MouseBatteryState"));

        SensorDefinition current_lvl = SensorDefinition( QString("/%1").arg(MouseBatteryRange), 0, MouseBatteryRange);

        updateValue("Mouse Battery Level","","",MouseBatteryLevel, current_lvl);
        updateValue("Mouse Battery Level (Max)","","",MouseBatteryRange);
        updateValue("Mouse Battery Level (Approx.%)","","",qRound(MouseBatteryLevel*100.0/MouseBatteryRange), pcnt_);
        updateValue("Mouse Battery State","","Logitech Style",reg.value("MouseBatteryState").toString());

        updateValue("Mouse Battery State","","LCDHost Style (Recommended)",batteryStates_[MouseBatteryRange][MouseBatteryLevel]);
        return successfulUpdate;
    }
}
