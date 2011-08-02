#include "LH_Aida64Data.h"
#include <QDebug>

#ifdef Q_WS_WIN
# include <windows.h>
#endif

LH_Aida64Data::LH_Aida64Data( LH_QtObject *parent, LH_MonitoringUI *ui,monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups)
{
    dataMode_ = dataMode;
    dataLoaded_ = false;
    aidaXml_ = QDomDocument("aidaData");

    ui_->reset(ui_mode_aida64);
    ui_->setVisible(mon_type | mon_group | mon_item);
}

LH_Aida64Data::~LH_Aida64Data()
{
}

QString getTypeName(QString tagName)
{
    if (tagName == "sys") return QString("System (%1)").arg(tagName);
    if (tagName == "temp") return QString("Temperatures (%1)").arg(tagName);
    if (tagName == "fan") return QString("Cooling Fans (%1)").arg(tagName);
    if (tagName == "duty") return QString("Fan Speeds (%1)").arg(tagName);
    if (tagName == "volt") return QString("Voltage Values (%1)").arg(tagName);
    return tagName;
}

bool LH_Aida64Data::getCount(int& count)
{
    QStringList groupItems;
    groupItems  << "Average" << "Lowest" << "Highest" << "All";

    count = 0;
    if (ui_->setup_value_item_name_->value() != "All")
        count = 1;
    else
        foreach(QString str, ui_->setup_value_item_->list() )
            if(!groupItems.contains(str)) count ++;
    return true;
}

bool LH_Aida64Data::getNames(QStringList& names)
{
    QStringList groupItems;
    groupItems  << "Average" << "Lowest" << "Highest" << "All";

    names.clear();
    if (ui_->setup_value_item_name_->value() != "All")
        names.append(ui_->setup_value_item_name_->value());
    else
        foreach(QString str, ui_->setup_value_item_->list() )
            if(!groupItems.contains(str)) names.append(str);
    return true;
}

monitoringDataType LH_Aida64Data::getType()
{
    if(ui_->setup_value_type_name_->value() == getTypeName("temp"))
        return mdtInt;
    if(ui_->setup_value_type_name_->value() == getTypeName("fan"))
        return mdtInt;
    if(ui_->setup_value_type_name_->value() == getTypeName("duty"))
        return mdtInt;
    if(ui_->setup_value_type_name_->value() == getTypeName("volt"))
        return mdtFloat;

    QString units;
    float value;
    QString resultText;
    if (getData(value, resultText, units))
    {
        if(resultText!="")
            return mdtText;
        else
            return mdtInt;
    }
    return mdtText;
}

int LH_Aida64Data::getThreshMax()
{
    if(ui_->setup_value_type_name_->value() == getTypeName("sys"))
        return 9999;
    if(ui_->setup_value_type_name_->value() == getTypeName("temp"))
        return 200;
    if(ui_->setup_value_type_name_->value() == getTypeName("fan"))
        return 9999;
    if(ui_->setup_value_type_name_->value() == getTypeName("duty"))
        return 9999;
    if(ui_->setup_value_type_name_->value() == getTypeName("volt"))
        return 20;

    return 9999;
}

void LH_Aida64Data::saveXML(QString fileName)
{
    QFile outFile( fileName );
    if(outFile.open( QIODevice::WriteOnly | QIODevice::Text ))
    {
        QTextStream stream( &outFile );
        stream << aidaXml_.toString();
        outFile.close();
    }
}

void LH_Aida64Data::updateLists()
{
    sensorTypes_.clear();

    //saveXML("aida.xml");
    QDomNode n = aidaXml_.documentElement().firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QString typeName = getTypeName(e.tagName());

            if(ui_->setup_value_type_name_->value()=="")
                ui_->setup_value_type_name_->setValue(typeName);

            sensorTypes_.add(typeName);
            if( typeName == ui_->setup_value_type_name_->value() )
            {
                QDomNodeList values = e.elementsByTagName("value");
                bool isNumeric = false;
                if(values.count()==1)
                    values.at(0).toElement().text().toFloat(&isNumeric);

                QDomNodeList labels = e.elementsByTagName("label");
                if(labels.count()==1 && ( ( (dataMode_ & mdmNumbers) && isNumeric ) || ( (dataMode_ & mdmText) && !isNumeric ) ) ) {
                    QString labelText = labels.at(0).toElement().text();

                    QString groupName = sensorTypes_.add(typeName, labelText).groupName;

                    if(ui_->setup_value_group_name_->value()=="")
                        ui_->setup_value_group_name_->setValue(groupName);
                    if( groupName == ui_->setup_value_group_name_->value() )
                        if(ui_->setup_value_item_name_->value()=="")
                            ui_->setup_value_item_name_->setValue(labelText);
                }
            }
        }
        n = n.nextSibling();
    }

    sensorTypes_.enhanceGroups(includeGroups_);

    sensorTypes_.refreshTypesList(ui_->setup_value_type_, ui_->setup_value_type_name_->value());
    sensorTypes_.refreshGroupsList(ui_->setup_value_group_, ui_->setup_value_type_name_->value(), ui_->setup_value_group_name_->value());
    sensorTypes_.refreshSensorsList(ui_->setup_value_item_, ui_->setup_value_type_name_->value(), ui_->setup_value_group_name_->value(), ui_->setup_value_item_name_->value());

}

bool LH_Aida64Data::loadXML(bool doListUpdate)
{
#ifdef Q_WS_WIN
    bool resultVal = true;

    const char* mapnameAida64  = "AIDA64_SensorValues";
    const char* mapnameEverest  = "EVEREST_SensorValues";

    // Create file mapping
    HANDLE filemap = (HANDLE)OpenFileMappingA(FILE_MAP_READ,0,mapnameAida64);
    if(filemap == NULL)
        filemap = (HANDLE)OpenFileMappingA(FILE_MAP_READ,0,mapnameEverest);

    // Get pointer
    if(filemap != NULL)
    {
        char* aidaData = (char*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);
        if (aidaData) {
            aidaXml_.setContent(QString("<data>%1</data>").arg(QString(aidaData)));
            if(doListUpdate) updateLists();
            UnmapViewOfFile(aidaData);
        } else
        {
            //qDebug()<<"unable to access data";
            resultVal = false;
        }
        CloseHandle(filemap);
    } else {
        //qDebug()<<"unable to map file";
        resultVal = false;
    }

    ui_->setReadonly(mon_type | mon_group | mon_item, !resultVal);

    if (!dataLoaded_ && resultVal)
    {
        dataLoaded_ = true;
        ui_->setTypeSelection();
        ui_->setGroupSelection();
        ui_->setItemSelection();
    }
    return resultVal;
#else
    Q_UNUSED(doListUpdate);
    return false;
#endif
}

bool LH_Aida64Data::getData(float& value, QString& text, QString& units)
{
    return getData(value, text, units, 0);
}

bool LH_Aida64Data::getData(float& value, QString& text, QString& units, int index)
{
    if (loadXML()) {
        getSelectedValue(aidaXml_.documentElement(), value, text, units, index);
        return true;
    } else
        return false;
}

void LH_Aida64Data::getSelectedValue(QDomElement docElem, float& value, QString& text, QString& units, int index)
{
    bool foundType = false;
    bool foundItem = false;

    QString targetType = ui_->setup_value_type_name_->value();
    QString targetItem = ui_->setup_value_item_name_->value();

    QStringList groupItems;
    groupItems  << "Average" << "Lowest" << "Highest" << "All";

    QStringList targetItems;
    if(groupItems.contains(targetItem))
    {
        foreach(QString str, ui_->setup_value_item_->list() )
            if(!groupItems.contains(str)) targetItems << str;
    } else
       targetItems << targetItem;
    is_group_ = (targetItem == "All");

    QList<float> values;
    values.append(0);
    int valueCount = 0;
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if(!e.isNull()) {
            QString typeName = getTypeName(e.tagName());
            if( typeName == targetType )
            {
                foundType = true;
                QDomNodeList labelNodes = e.elementsByTagName("label");
                if(labelNodes.count()==1)
                {
                    QString itemName = labelNodes.at(0).toElement().text();
                    if( targetItems.contains(itemName) )
                    {
                        foundItem = true;
                        QDomNodeList valueNodes = e.elementsByTagName("value");
                        if(valueNodes.count()==1)
                        {
                            bool ok;
                            float elementVal = valueNodes.at(0).toElement().text().toFloat(&ok);
                            if(!ok)
                                text = valueNodes.at(0).toElement().text();
                            else
                                if(++valueCount==1)
                                    values[0] = elementVal;
                                else
                                    switch(groupItems.indexOf(targetItem))
                                    {
                                    case 0: //avg
                                        values[0] += elementVal;
                                        break;
                                    case 1: //low
                                        values[0] = qMin(values[0], elementVal);
                                        break;
                                    case 2: //high
                                        values[0] = qMax(values[0], elementVal);
                                        break;
                                    case 3: //all
                                        values.append( elementVal );
                                        break;
                                    }

                            units="";
                            if (e.tagName() == "sys") {
                                if (itemName.endsWith("FSB"))  units="mhz";
                                if (itemName.endsWith("Clock"))  units="mhz";
                                if (itemName.endsWith("Utilization"))  units="%";
                                if (itemName.endsWith("Memory"))  units="MB";
                            }
                            if (e.tagName() == "temp") units="°C";
                            if (e.tagName() == "fan") units="rpm";
                            //if (e.tagName() == "duty") units="";
                            if (e.tagName() == "volt") units="V";

                        }
                    }
                }
            }
        }
        n = n.nextSibling();
    }
    if(groupItems.indexOf(targetItem) == 0) values[0] /= valueCount;
    if(index<values.count())
        value = values[index];

    if(!foundType || !foundItem) {
        /*if(!foundType)
            qWarning() << "LH_Aida64: Data type not found: " << ui_->setup_value_type_name_->value();
        else
            qWarning() << "LH_Aida64: Data item not found: " << ui_->setup_value_item_name_->value();*/
        value = 0;
        text  = "N/A";
        units = "";
    }

    return ;
}
