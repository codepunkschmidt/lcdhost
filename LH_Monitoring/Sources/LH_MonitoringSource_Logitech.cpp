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

    pcnt_ = SensorDefinition( "%" );
}


bool LH_MonitoringSource_Logitech::doUpdate()
{
    QSettings reg("HKEY_CURRENT_USER\\Software\\CodeLeap\\LogitechMonitoring", QSettings::NativeFormat);

    setDataAvailable(reg.contains("MonitoringInstalled") && reg.value("MonitoringInstalled").toInt() == 1 && reg.contains("MouseBatteryRange") && reg.value("MouseBatteryRange").toInt() != 0);
    if(!dataAvailable())
        return false;
    else
    {
        int MouseBatteryLevel = 0;
        int MouseBatteryRange = 0;
        QString MouseBatteryState = "N/A";

        bool successfulUpdate = (reg.contains("MouseBatteryLevel")|| reg.contains("MouseBatteryRange") || reg.contains("MouseBatteryState"));

        if(reg.contains("MouseBatteryLevel"))
            MouseBatteryLevel = reg.value("MouseBatteryLevel").toInt();
        if(reg.contains("MouseBatteryRange"))
            MouseBatteryRange = reg.value("MouseBatteryRange").toInt();
        if(reg.contains("MouseBatteryState"))
            MouseBatteryState = reg.value("MouseBatteryState").toString();

        SensorDefinition current_lvl(
                    QString("/%1").arg(MouseBatteryRange),
                    minmax(OptionalValue(true, 0), OptionalValue(true, MouseBatteryRange)));

        updateValue("Mouse Battery Level","","",MouseBatteryLevel, current_lvl);
        updateValue("Mouse Battery Level (Max)","","",MouseBatteryRange);
        updateValue("Mouse Battery Level (Approx.%)","","",qRound(MouseBatteryLevel*100.0/MouseBatteryRange), pcnt_);
        updateValue("Mouse Battery State","","Logitech Style",reg.value("MouseBatteryState").toString());
        updateValue("Mouse Battery State","","LCDHost Style (Recommended)",batteryStates_[MouseBatteryRange][MouseBatteryLevel]);
        return successfulUpdate;
    }
}
