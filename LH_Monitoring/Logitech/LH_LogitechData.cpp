#include "LH_LogitechData.h"
#include <QDebug>
#include <QRegExp>
#include <algorithm>

int logitechEnabled_ = -1;

LH_LogitechData::LH_LogitechData(LH_QtObject *parent, monitoringDataMode dataMode) : LH_MonitoringData( parent )
{
    QStringList valueTypes;
    if(dataMode & mdmNumbers) valueTypes.append("Mouse Battery Level (Approx.%)");
    if(dataMode & mdmNumbers) valueTypes.append("Mouse Battery Level");
    if(dataMode & mdmNumbers) valueTypes.append("Mouse Battery Level (Max)");
    if(dataMode & mdmText) valueTypes.append("Mouse Battery State (Logitech)");
    if(dataMode & mdmText) valueTypes.append("Mouse Battery State (LCDHost)");

    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium-High"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium-High"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium"<<"Medium-High"<<"High");
    batteryStates.append(QStringList()<<"Unknown"<<"Low"<<"Medium-Low"<<"Medium"<<"Medium"<<"Medium"<<"Medium-High"<<"High");


    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Item", valueTypes);
    setup_value_type_->setHelp( "<p>The value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Item)",0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_type_index_->setOrder(-4);

    if (valueTypes.count()==1) {
        setup_value_type_->setValue(0);
        setup_value_type_->setFlag(LH_FLAG_HIDDEN, true);
    }

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_type_index_, SIGNAL(changed()), parent, SLOT(setTypeSelection()) );


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

    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery Level")
    {
        if (reg.contains("MouseBatteryLevel"))
            value = reg.value("MouseBatteryLevel").toInt()-1;
        else
            resultVal = false;
    }
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery Level (Max)")
    {
        if (reg.contains("MouseBatteryRange"))
            value = reg.value("MouseBatteryRange").toInt()-1;
        else
            resultVal = false;
    }
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery State (Logitech)")
    {
        if (reg.contains("MouseBatteryState"))
            text = reg.value("MouseBatteryState").toString();
        else
        {
            text = "N/A";
            resultVal = false;
        }
    }
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery State (LCDHost)")
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
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery Level (Approx.%)")
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

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    return resultVal;
}

monitoringDataType LH_LogitechData::getType()
{
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery State (Logitech)")
        return mdtText;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Mouse Battery State (LCDHost)")
        return mdtText;

    return mdtInt;
}

void LH_LogitechData::setTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_LogitechData::changeTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_index_->setValue(setup_value_type_->value());
}

bool LH_LogitechData::monitoringEnabled()
{
    return true;
    /*if(logitechEnabled_==-1)
    {
        QSettings reg("HKEY_CURRENT_USER\\Software\\CodeLeap\\LogitechMonitoring", QSettings::NativeFormat);
        int installedVersion = 0;
        if (reg.contains("MonitoringInstalled"))
            installedVersion = reg.value("MonitoringInstalled").toInt();
        logitechEnabled_ = (installedVersion==1? 1 : 0);
    }
    return (logitechEnabled_ == 1);*/
}
