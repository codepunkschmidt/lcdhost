#include "LH_MonitoringSource_HWMonitor.h"

#include <QSettings>
#include <QDebug>
//#include <algorithm>

LH_MonitoringSource_HWMonitor::LH_MonitoringSource_HWMonitor(LH_QtObject *parent): LH_MonitoringSource(parent, "HWMonitor (+ HWMonTray)") {}

bool LH_MonitoringSource_HWMonitor::doUpdate()
{
#ifndef Q_OS_WIN
    return false;
#else
    bool resultVal = false;

    QRegExp rx("([FTPV])\\[(.*)\\]");
    QRegExp rx2("(.*)-([0-9]*)");
    QRegExp rx3("(.*)#([0-9]*)");

    QList<QString> aggGroups;

    QSettings reg("HKEY_CURRENT_USER\\Software\\CPUID\\HWMonitor\\VistaSidebar", QSettings::NativeFormat);
    setDataAvailable(reg.childKeys().count()!=0);
    if(dataAvailable())
    {
        resultVal = true;

        QStringList keysList = reg.childKeys();
        keysList.sort();
        foreach(QString key, keysList)
        {
            if(rx.indexIn(key) == -1)
                continue;
            QString keyType = rx.cap(1);
            QString keyName = rx.cap(2);
            QString groupName = "";

            if(rx2.indexIn(keyName) != -1)
            {
                if(rx2.cap(2)=="0" && !reg.childKeys().contains(QString("%1[%2-1]").arg(keyType,rx2.cap(1))))
                {
                    keyName = rx2.cap(1);
                    if(rx3.indexIn(keyName) != -1)
                        groupName = rx3.cap(1);
                }
                else
                    keyName = QString("%1 (%2)").arg(rx2.cap(1), rx2.cap(2));
            }
            if(groupName=="")
                groupName = keyName;

            QString units = ""; QString type="Other";

            if(keyType=="F") {units="rpm"; type = "Fan Speed";}
            if(keyType=="T") {units="\176C"; type = "Temperature";}
            if(keyType=="P") {units="W"; type = "Powers";}
            if(keyType=="V") {units="V"; type = "Voltage";}

            QVariant val = reg.value(key);

            if(!aggGroups.contains(QString("%1|~>|%2").arg(type).arg(groupName)))
                aggGroups.append(QString("%1|~>|%2").arg(type).arg(groupName));

            updateValue(type,groupName,keyName,val,SensorDefinition(units));
        }
        foreach(QString aggGroup, aggGroups)
        {
            QStringList aggItem = aggGroup.split("|~>|");
            updateAggregates(aggItem[0],aggItem[1]);
        }

    }
    return resultVal;
#endif
}
