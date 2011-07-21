#include "LH_LogitechData.h"
#include <QDebug>
#include <QRegExp>
#include <algorithm>
#include <QSettings>

LH_LogitechData::LH_LogitechData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    Q_UNUSED(includeGroups);

    ui_->reset();
    ui_->setVisible(mon_item);

    if(dataMode & mdmNumbers) ui_->append(mon_item, "Mouse Battery Level (Approx.%)");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Mouse Battery Level");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Mouse Battery Level (Max)");
    if(dataMode & mdmText) ui_->append(mon_item, "Mouse Battery State (Logitech)");
    if(dataMode & mdmText) ui_->append(mon_item, "Mouse Battery State (LCDHost)");

    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium-High"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium-High"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium"<<"Medium-High"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium"<<"Medium"<<"Medium-High"<<"High");

    ui_->refresh(mon_item);
}

LH_LogitechData::~LH_LogitechData()
{

}

bool LH_LogitechData::getData(float& value, QString& text, QString& units)
{
    bool resultVal = true;

    value = 0;
    text = "";
    units = "";

    QSettings reg("HKEY_CURRENT_USER\\Software\\CodeLeap\\LogitechMonitoring", QSettings::NativeFormat);

    if(ui_->valueText(mon_item) == "Mouse Battery Level")
    {
        if (reg.contains("MouseBatteryLevel"))
            value = reg.value("MouseBatteryLevel").toInt()-1;
        else
            resultVal = false;
    }
    if(ui_->valueText(mon_item) == "Mouse Battery Level (Max)")
    {
        if (reg.contains("MouseBatteryRange"))
            value = reg.value("MouseBatteryRange").toInt()-1;
        else
            resultVal = false;
    }
    if(ui_->valueText(mon_item) == "Mouse Battery State (Logitech)")
    {
        if (reg.contains("MouseBatteryState"))
            text = reg.value("MouseBatteryState").toString();
        else
        {
            text = "N/A";
            resultVal = false;
        }
    }
    if(ui_->valueText(mon_item) == "Mouse Battery State (LCDHost)")
    {
        if (reg.contains("MouseBatteryLevel") && reg.contains("MouseBatteryRange"))
        {
            if (reg.value("MouseBatteryRange").toInt() != 0)
                text = batteryStates[reg.value("MouseBatteryRange").toInt()-2][reg.value("MouseBatteryLevel").toInt()];
            else
                text = batteryStates[0][0];
        }
        else
        {
            text = "N/A";
            resultVal = false;
        }
    }
    if(ui_->valueText(mon_item) == "Mouse Battery Level (Approx.%)")
    {
        if (reg.contains("MouseBatteryLevel") && reg.contains("MouseBatteryRange"))
        {
            if (reg.value("MouseBatteryLevel").toInt() != 0)
            {
                value = qRound((reg.value("MouseBatteryLevel").toInt())*100.0/(reg.value("MouseBatteryRange").toInt()));
                units = "%";
            }
            else
                resultVal = false;
        }
        else
            resultVal = false;
    }

    ui_->setReadonly(mon_item, !resultVal);
    return resultVal;
}

monitoringDataType LH_LogitechData::getType()
{
    if(ui_->valueText(mon_item) == "Mouse Battery State (Logitech)")
        return mdtText;
    if(ui_->valueText(mon_item) == "Mouse Battery State (LCDHost)")
        return mdtText;

    return mdtInt;
}
