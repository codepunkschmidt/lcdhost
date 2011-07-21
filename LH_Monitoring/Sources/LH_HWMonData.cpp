#include "LH_HWMonData.h"
#include <QDebug>
#include <QRegExp>
#include <algorithm>

enum single_value_type
{
    gv_default = 0,
    gv_lowest = 1,
    gv_highest = 2,
    gv_average = 3
};

LH_HWMonData::LH_HWMonData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    ui_->reset();
    ui_->setVisible(mon_type | mon_item);
    ui_->append(mon_type, "Fan Speed");
    ui_->append(mon_type, "Temperature");
    ui_->append(mon_type, "Powers");
    ui_->append(mon_type, "Voltage");
    ui_->refresh(mon_type | mon_item);
}

bool LH_HWMonData::getCount(int& count)
{
    float value; QString text; QString units;
    count = 0;
    return getData(value, text, units, count, 0);
}

bool LH_HWMonData::getData(float& value, QString& text, QString& units)
{
    int count;
    return getData(value, text, units, count, 0);
}
bool LH_HWMonData::getData(float& value, QString& text, QString& units, int index)
{
    int count;
    return getData(value, text, units, count, index);
}
bool LH_HWMonData::getData(float& value, QString& text, QString& units, int& count, int index)
{
    return getSelectedValue(value, text, units, count, index);
}


bool LH_HWMonData::getSelectedValue(float& value, QString& text, QString& units, int& count, int index)
{
    bool resultVal = true;

    value = 0;
    text = "";
    units = "";
    count = 0;

    single_value_type gvt = gv_default;

    QSettings reg("HKEY_CURRENT_USER\\Software\\CPUID\\HWMonitor\\VistaSidebar", QSettings::NativeFormat);
    if (ui_->count(mon_item) == 0)
        loadSensorList(&reg);

    if(ui_->value(mon_item)<sensorKeys_.length())
    {
        QStringList items = sensorKeys_.at(ui_->value(mon_item));

        if(ui_->valueText(mon_item).endsWith("(All)"))
            count = items.count();
        else
        {
            count = 1;
            if(ui_->valueText(mon_item).endsWith("(Highest)"))
                gvt = gv_highest;
            if(ui_->valueText(mon_item).endsWith("(Lowest)"))
                gvt = gv_lowest;
            if(ui_->valueText(mon_item).endsWith("(Average)"))
                gvt = gv_average;
        }
        if (index>=count) index = count-1;
        is_group_ = (count>1);

        QString key;
        for(int i = 0; i<items.count(); i++)
        {
            if(gvt==gv_default)
                key = items.at(index);
            else
                key = items.at(i);

            if (reg.contains(key))
            {
                text = reg.value(key).toString();
                if(text.contains("%"))
                {
                    text = text.remove("%").trimmed();
                    units = "%";
                }
                else
                {
                    QRegExp rx("([FTPV])\\[([^\\]]*)\\]");
                    if(rx.indexIn(key) != -1)
                    {
                        if(rx.cap(1)=="F")
                            units="rpm";
                        if(rx.cap(1)=="T")
                            units="°C";
                        if(rx.cap(1)=="P")
                            units="W";
                        if(rx.cap(1)=="V")
                            units="V";
                    }
                }
                switch(gvt)
                {
                case gv_default:
                    value = text.toFloat();
                    break;
                case gv_lowest:
                    if(i==0)
                        value = text.toFloat();
                    else
                        value = (value<=text.toFloat()? value : text.toFloat());
                    break;
                case gv_highest:
                    if(i==0)
                        value = text.toFloat();
                    else
                        value = (value>=text.toFloat()? value : text.toFloat());
                    break;
                case gv_average:
                    value += text.toFloat();
                    break;
                }
                if(gvt==gv_default)
                    break;
            }
            else
                resultVal = false;
        }
        if(gvt==gv_average)
            value /= items.count();
    }
    else
        resultVal = false;
    if(gvt!=gv_default)
        text = QString::number(value);
    ui_->setReadonly(mon_type | mon_item, !resultVal);
    return resultVal;
}


monitoringDataType LH_HWMonData::getType()
{
    if(ui_->valueText(mon_type) == "Fan Speed") return mdtInt;
    if(ui_->valueText(mon_type) == "Temperature") return mdtInt;
    if(ui_->valueText(mon_type) == "Powers") return mdtFloat;
    if(ui_->valueText(mon_type) == "Voltage") return mdtFloat;

    return mdtInt;
}

void LH_HWMonData::loadSensorList(QSettings *reg)
{
    ui_->clear(mon_item);
    sensorKeys_.clear();
    QHash<QString, QStringList> groupedKeys;
    QHash<QString, int> lastEntries;

    QString itemType = "";
    if(ui_->valueText(mon_type) == "Fan Speed") itemType = "F";
    if(ui_->valueText(mon_type) == "Temperature") itemType = "T";
    if(ui_->valueText(mon_type) == "Powers") itemType = "P";
    if(ui_->valueText(mon_type) == "Voltage") itemType = "V";

    QRegExp rx("([FTPV])\\[(.*)\\]");
    QRegExp rx2("(.*)-([0-9]*)");
    QRegExp rx3("(.*)#([0-9]*)");
    QStringList keysList = reg->childKeys();
    keysList.sort();
    foreach(QString key, keysList)
    {
        if(rx.indexIn(key) == -1)
            continue;
        QString keyType = rx.cap(1);
        QString keyName = rx.cap(2);
        if(keyType!=itemType)
            continue;

        if(rx2.indexIn(keyName) != -1)
        {
            if(rx2.cap(2)=="0" && !reg->childKeys().contains(QString("%1[%2-1]").arg(itemType,rx2.cap(1))))
            {
                keyName = rx2.cap(1);
                if(rx3.indexIn(keyName) != -1)
                {
                    QString groupName = rx3.cap(1);
                    QStringList list;

                    if(groupedKeys.contains(groupName))
                    {
                        list = groupedKeys.value(groupName);
                        groupedKeys.remove(groupName);
                        lastEntries.remove(groupName);
                    }
                    list.append(key);
                    groupedKeys.insert(groupName, list);
                    lastEntries.insert(groupName, ui_->count(mon_item));
                }
            }
            else
                keyName = QString("%1 (%2)").arg(rx2.cap(1), rx2.cap(2));
        }
        ui_->append(mon_item, keyName);
        sensorKeys_.append(QStringList()<<key);
    }
    foreach(QString key, groupedKeys.keys())
    {
        if(groupedKeys.value(key).length()!=1)
        {
            int i = lastEntries.value(key)+1;
            if(includeGroups_)
            {
                ui_->insert(mon_item, i, QString("%1 (All)").arg(key));
                sensorKeys_.insert(i, QStringList()<<groupedKeys.value(key));
            }
            ui_->insert(mon_item, i, QString("%1 (Average)").arg(key));
            sensorKeys_.insert(i, QStringList()<<groupedKeys.value(key));
            ui_->insert(mon_item, i, QString("%1 (Highest)").arg(key));
            sensorKeys_.insert(i, QStringList()<<groupedKeys.value(key));
            ui_->insert(mon_item, i, QString("%1 (Lowest)").arg(key));
            sensorKeys_.insert(i, QStringList()<<groupedKeys.value(key));
        }
    }

    ui_->refresh(mon_item);
    ui_->setValue(mon_item, ui_->setup_value_item_index_->value());
}
