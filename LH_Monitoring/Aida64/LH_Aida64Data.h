#ifndef LH_AIDA64DATA_H
#define LH_AIDA64DATA_H

#include "../_Templates/LH_MonitoringData.h"
#include <windows.h>

#include <QtXML>
#include <QRegExp>
#include <algorithm>

struct sensorsList
{
    QStringList names;
};

struct sensorGroupsList
{
    QStringList names;
    QList<sensorsList> sensors;
};

struct addedDetail
{
    QString typeName;
    QString groupName;
    QString sensorName;
};

class sensorTypesList
{
    QStringList names;
    QList<sensorGroupsList> groups;
    QString reverse(QString str)
    {
        QByteArray ba = str.toAscii();
        char *d = ba.data();
        std::reverse(d, d+str.length());
        return QString(d);
    }

    bool refreshList( QStringList list, LH_Qt_QStringList *listBox, QString selectedValue )
    {
        if (list != listBox->list())
        {
            listBox->list().clear();
            int selIndex = -1;
            foreach(QString str, list)
            {
                listBox->list().append(str);
                if(selectedValue==str.remove(" [Unavailable]"))
                    selIndex = listBox->list().count()-1;
            }
            listBox->setValue(selIndex);
            listBox->refreshList();
            return true;
        } else
            return false;
    }

public:
    void clear()
    {
        names.clear();
        groups.clear();
    }

    addedDetail add(QString typeName, QString sensorName = "")
    {
        addedDetail result;
        addedDetail result2;
        result.typeName = typeName;
        result2.typeName = typeName;
        if (!names.contains(typeName))
        {
            names.append(typeName);
            groups.append( sensorGroupsList() );
        }
        int typID = names.indexOf(typeName);
        if(sensorName != "")
        {
            QRegExp rx = QRegExp("([^0-9]*)([0-9]+#?\\s?)(.*)");
            rx.setPatternSyntax(QRegExp::RegExp2);

            result.sensorName = sensorName;
            result.groupName = (rx.indexIn(reverse(sensorName))==-1 ? sensorName : reverse(rx.cap(3)) + reverse(rx.cap(1)) );

            result2.sensorName = sensorName;
            result2.groupName = (rx.indexIn(reverse(result.groupName))==-1 ? result.groupName : reverse(rx.cap(3)) + reverse(rx.cap(1)) );

            if(result2.groupName!=result.groupName)
            {
                if(!groups[typID].names.contains(result2.groupName))
                {
                    groups[typID].names.append(result2.groupName);
                    groups[typID].sensors.append( sensorsList() );
                }
                int grpID2 = groups[typID].names.indexOf(result2.groupName);
                if(!groups[typID].sensors[grpID2].names.contains(sensorName))
                    groups[typID].sensors[grpID2].names.append(sensorName);
            }

            if(!groups[typID].names.contains(result.groupName))
            {
                groups[typID].names.append(result.groupName);
                groups[typID].sensors.append( sensorsList() );
            }
            int grpID = groups[typID].names.indexOf(result.groupName);
            if(!groups[typID].sensors[grpID].names.contains(sensorName))
                groups[typID].sensors[grpID].names.append(sensorName);
        }
        return result2;
    }

    void enhanceGroups(bool includeGroups)
    {
        for(int i=0; i<groups.count(); i++)
        {
            for(int j=groups[i].sensors.count()-2; j>=0; j--)
                if (groups[i].sensors[j].names == groups[i].sensors[j+1].names)
                {
                    //remove duplicate groups
                    groups[i].sensors.removeAt(j);
                    groups[i].names.removeAt(j);
                }

            for(int j=0; j<groups[i].sensors.count(); j++)
            {
                if (groups[i].sensors[j].names.count()==1)
                    //rename groups with one entry to be the same as that single member
                    groups[i].names[j] = groups[i].sensors[j].names[0];
                else
                {
                    // add group values
                    groups[i].sensors[j].names.append("Average");
                    groups[i].sensors[j].names.append("Lowest");
                    groups[i].sensors[j].names.append("Highest");
                    if(includeGroups)
                        groups[i].sensors[j].names.append("All");
                }
            }
        }
    }

    bool refreshTypesList( LH_Qt_QStringList *listBox, QString selectedType = "" )
    {
        return refreshList(names, listBox, selectedType);
    }

    bool refreshGroupsList( LH_Qt_QStringList *listBox, QString selectedType, QString selectedGroup = "" )
    {
        int typID = names.indexOf(selectedType);
        if(typID==-1)
            return false;
        else
            return refreshList(groups[typID].names, listBox, selectedGroup);
    }

    bool refreshSensorsList( LH_Qt_QStringList *listBox, QString selectedType, QString selectedGroup, QString selectedSensor = "" )
    {
        int typID = names.indexOf(selectedType);
        if(typID==-1) return false;

        int grpID = groups[typID].names.indexOf(selectedGroup);
        if(grpID==-1) return false;

        return refreshList(groups[typID].sensors[grpID].names, listBox, selectedSensor);
    }

};


class LH_Aida64Data: public LH_MonitoringData
{
    void saveXML(QString fileName);
    void updateLists();
    QDomDocument aidaXml_;
    bool dataLoaded_;
    monitoringDataMode dataMode_;

    sensorTypesList sensorTypes_;

    void getSelectedValue(QDomElement docElem, float& value, QString& text, QString& units, int index);

protected:
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    LH_Qt_QString *setup_value_type_name_;
    LH_Qt_QString *setup_value_group_name_;
    LH_Qt_QString *setup_value_sensor_name_;

    bool loadXML(bool doListUpdate = true);

public:

    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_QStringList *setup_value_group_;
    LH_Qt_QStringList *setup_value_sensor_;

    LH_Aida64Data( LH_QtObject *parent, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false);
    ~LH_Aida64Data();

    QString appName() { return "Aida64"; }
    bool getCount(int& count);
    bool getNames(QStringList& names);
    monitoringDataType getType();
    int getThreshMax();

    void setTypeSelection();
    void changeTypeSelection();
    void setGroupSelection();
    void changeGroupSelection();
    void setItemSelection();
    void changeItemSelection();
};

#endif // LH_AIDA64DATA_H
