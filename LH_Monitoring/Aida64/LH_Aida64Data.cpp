#include "LH_Aida64Data.h"
#include <QDebug>

LH_Aida64Data::LH_Aida64Data( LH_QtObject *parent, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, includeGroups)
{
    dataMode_ = dataMode;
    dataLoaded_ = false;
    aidaXml_ = QDomDocument("aidaData");

    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Type", QStringList(), LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-8);

    setup_value_type_name_ = new LH_Qt_QString(parent,"(Value Type)", "", LH_FLAG_HIDDEN);
    setup_value_type_name_->setOrder(-7);

    setup_value_group_ = new LH_Qt_QStringList(parent, "Value Group", QStringList(), LH_FLAG_READONLY);
    setup_value_group_->setHelp( "<p>The value to display.</p>");
    setup_value_group_->setOrder(-6);

    setup_value_group_name_ = new LH_Qt_QString(parent,"(Value Group)", "", LH_FLAG_HIDDEN);
    setup_value_group_name_->setOrder(-6);

    setup_value_sensor_ = new LH_Qt_QStringList(parent, "Value Item", QStringList(), LH_FLAG_READONLY);
    setup_value_sensor_->setHelp( "<p>The specific sensor to display.</p>");
    setup_value_sensor_->setOrder(-5);

    setup_value_sensor_name_ = new LH_Qt_QString(parent,"(Value Item)", "", LH_FLAG_HIDDEN);
    setup_value_sensor_name_->setOrder(-5);

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_type_name_, SIGNAL(changed()), parent, SLOT(setTypeSelection()) );
    parent->connect(setup_value_group_, SIGNAL(changed()), parent, SLOT(changeGroupSelection()) );
    parent->connect(setup_value_group_name_, SIGNAL(changed()), parent, SLOT(setGroupSelection()) );
    parent->connect(setup_value_sensor_, SIGNAL(changed()), parent, SLOT(changeItemSelection()) );
    parent->connect(setup_value_sensor_name_, SIGNAL(changed()), parent, SLOT(setItemSelection()) );
}

LH_Aida64Data::~LH_Aida64Data()
{
    return;
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
    if (setup_value_sensor_name_->value() != "All")
        count = 1;
    else
        foreach(QString str, setup_value_sensor_->list() )
            if(!groupItems.contains(str)) count ++;
    return true;
}

bool LH_Aida64Data::getNames(QStringList& names)
{
    QStringList groupItems;
    groupItems  << "Average" << "Lowest" << "Highest" << "All";

    names.clear();
    if (setup_value_sensor_name_->value() != "All")
        names.append(setup_value_sensor_name_->value());
    else
        foreach(QString str, setup_value_sensor_->list() )
            if(!groupItems.contains(str)) names.append(str);
    return true;
}

monitoringDataType LH_Aida64Data::getType()
{
    if(setup_value_type_name_->value() == getTypeName("temp"))
        return mdtInt;
    if(setup_value_type_name_->value() == getTypeName("fan"))
        return mdtInt;
    if(setup_value_type_name_->value() == getTypeName("duty"))
        return mdtInt;
    if(setup_value_type_name_->value() == getTypeName("volt"))
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
    if(setup_value_type_name_->value() == getTypeName("sys"))
        return 9999;
    if(setup_value_type_name_->value() == getTypeName("temp"))
        return 200;
    if(setup_value_type_name_->value() == getTypeName("fan"))
        return 9999;
    if(setup_value_type_name_->value() == getTypeName("duty"))
        return 9999;
    if(setup_value_type_name_->value() == getTypeName("volt"))
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

            if(setup_value_type_name_->value()=="")
                setup_value_type_name_->setValue(typeName);

            sensorTypes_.add(typeName);
            if( typeName == setup_value_type_name_->value() )
            {
                QDomNodeList values = e.elementsByTagName("value");
                bool isNumeric = false;
                if(values.count()==1)
                    values.at(0).toElement().text().toFloat(&isNumeric);

                QDomNodeList labels = e.elementsByTagName("label");
                if(labels.count()==1 && ( ( (dataMode_ & mdmNumbers) && isNumeric ) || ( (dataMode_ & mdmText) && !isNumeric ) ) ) {
                    QString labelText = labels.at(0).toElement().text();

                    QString groupName = sensorTypes_.add(typeName, labelText).groupName;

                    if(setup_value_group_name_->value()=="")
                        setup_value_group_name_->setValue(groupName);
                    if( groupName == setup_value_group_name_->value() )
                        if(setup_value_sensor_name_->value()=="")
                            setup_value_sensor_name_->setValue(labelText);
                }
            }
        }
        n = n.nextSibling();
    }

    sensorTypes_.enhanceGroups(includeGroups_);

    sensorTypes_.refreshTypesList(setup_value_type_, setup_value_type_name_->value());
    sensorTypes_.refreshGroupsList(setup_value_group_, setup_value_type_name_->value(), setup_value_group_name_->value());
    sensorTypes_.refreshSensorsList(setup_value_sensor_, setup_value_type_name_->value(), setup_value_group_name_->value(), setup_value_sensor_name_->value());

}

bool LH_Aida64Data::loadXML(bool doListUpdate)
{
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

    //qDebug() << setup_value_type_->flags();
    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    setup_value_group_->setFlag(LH_FLAG_READONLY, !resultVal);
    setup_value_sensor_->setFlag(LH_FLAG_READONLY, !resultVal);

    if (!dataLoaded_ && resultVal)
    {
        dataLoaded_ = true;
        setTypeSelection();
        setGroupSelection();
        setItemSelection();
    }
    return resultVal;
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

    QString targetType = setup_value_type_name_->value();
    QString targetItem = setup_value_sensor_name_->value();

    QStringList groupItems;
    groupItems  << "Average" << "Lowest" << "Highest" << "All";

    QStringList targetItems;
    if(groupItems.contains(targetItem))
    {
        foreach(QString str, setup_value_sensor_->list() )
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
            qWarning() << "LH_Aida64: Data type not found: " << setup_value_type_name_->value();
        else
            qWarning() << "LH_Aida64: Data item not found: " << setup_value_sensor_name_->value();*/
        value = 0;
        text  = "N/A";
        units = "";
    }

    return ;
}

void LH_Aida64Data::setTypeSelection()
{
    if (loadXML(true))
    {
        for(int i = 0; i<setup_value_type_->list().length(); i++)
            if(QString(setup_value_type_->list().at(i)).remove(" [Unavailable]") == setup_value_type_name_->value())
                setup_value_type_->setValue(i);
    } else
    {
        setup_value_type_->list().clear();
        setup_value_type_->list().append(QString(setup_value_type_name_->value() + " [Unavailable]").trimmed());
        setup_value_type_->refreshList();
        setup_value_type_->setValue(setup_value_type_->list().count()-1);
    }
}

void LH_Aida64Data::changeTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
    {
        if(setup_value_type_->value()!=-1)
            if(setup_value_type_->value() < setup_value_type_->list().count())
                setup_value_type_name_->setValue(QString(setup_value_type_->list().at(setup_value_type_->value())).remove(" [Unavailable]"));
        setup_value_group_name_->setValue("");
        setup_value_sensor_name_->setValue("");
        updateLists();
        changeGroupSelection();
    }
}

void LH_Aida64Data::setGroupSelection()
{
    if (loadXML(true))
    {
        for(int i = 0; i<setup_value_group_->list().length(); i++)
            if(QString(setup_value_group_->list().at(i)).remove(" [Unavailable]") == setup_value_sensor_name_->value())
                setup_value_group_->setValue(i);
    } else
    {
        setup_value_group_->list().clear();
        setup_value_group_->list().append(QString(setup_value_group_name_->value() + " [Unavailable]").trimmed());
        setup_value_group_->refreshList();
        setup_value_group_->setValue(setup_value_group_->list().count()-1);
    }
}

void LH_Aida64Data::changeGroupSelection()
{
    if (setup_value_group_->list().length()!=0)
    {
        if(setup_value_group_->value()!=-1)
            if(setup_value_group_->value() < setup_value_group_->list().count())
                setup_value_group_name_->setValue(setup_value_group_->list().at(setup_value_group_->value()));
        setup_value_sensor_name_->setValue("");
        updateLists();
        changeItemSelection();
        setup_value_sensor_->setFlag(LH_FLAG_HIDDEN, (setup_value_sensor_->list().count()<=1) );
    }
}

void LH_Aida64Data::setItemSelection()
{
    if (loadXML(true))
    {
        for(int i = 0; i<setup_value_sensor_->list().length(); i++)
            if(QString(setup_value_sensor_->list().at(i)).remove(" [Unavailable]") == setup_value_sensor_name_->value())
                setup_value_sensor_->setValue(i);
    } else
    {
        setup_value_sensor_->list().clear();
        setup_value_sensor_->list().append(QString(setup_value_sensor_name_->value() + " [Unavailable]").trimmed());
        setup_value_sensor_->refreshList();
        setup_value_sensor_->setValue(setup_value_sensor_->list().count()-1);
    }
}

void LH_Aida64Data::changeItemSelection()
{
    if (setup_value_sensor_->list().length()!=0)
        if(setup_value_sensor_->value()!=-1)
            if(setup_value_sensor_->value()<setup_value_sensor_->list().count())
                setup_value_sensor_name_->setValue(setup_value_sensor_->list().at(setup_value_sensor_->value()));
    is_group_ = (setup_value_sensor_name_->value() == "All");
}


