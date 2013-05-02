#include "LH_MonitoringSource_Aida64.h"
#include <QRegExp>
#include <QSharedMemory>

#ifdef Q_OS_WIN
# include <windows.h>
#endif

LH_MonitoringSource_Aida64::LH_MonitoringSource_Aida64(LH_QtObject *parent): LH_MonitoringSource(parent, "Aida64") {
    aidaXml_ = QDomDocument("aidaData");
}

bool LH_MonitoringSource_Aida64::doUpdate()
{
#ifdef Q_OS_WIN
    bool resultVal = true;

    const char* mapnameAida64  = "AIDA64_SensorValues";
    const char* mapnameEverest  = "EVEREST_SensorValues";

    // Create file mapping
    HANDLE filemap = (HANDLE)OpenFileMappingA(FILE_MAP_READ,0,mapnameAida64);
    if(filemap == NULL)
        filemap = (HANDLE)OpenFileMappingA(FILE_MAP_READ,0,mapnameEverest);

    setDataAvailable(filemap != NULL);
    if(dataAvailable())
    {
        char* aidaData = (char*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);
        if (aidaData) {
            aidaXml_.setContent(QString("<data>%1</data>").arg(QString(aidaData)));

            QRegExp rx = QRegExp("([^0-9]*)([0-9]+#?\\s?)(.*)");
            rx.setPatternSyntax(QRegExp::RegExp2);

            QDomNode n = aidaXml_.documentElement().firstChild();
            while(!n.isNull()) {
                QDomElement e = n.toElement(); // try to convert the node to an element.
                if(!e.isNull()) {
                    QDomNodeList labelNodes = e.elementsByTagName("label");
                    QDomNodeList valueNodes = e.elementsByTagName("value");

                    if(labelNodes.count()==1 && valueNodes.count()==1)
                    {

                        QString typeName = e.tagName();
                        QString itemName = labelNodes.at(0).toElement().text();
                        QString groupName = (rx.indexIn(reverse(itemName))==-1 ? itemName : reverse(rx.cap(3)) + reverse(rx.cap(1)) );
                        QString valueString = valueNodes.at(0).toElement().text();
                        QString units="";

                        if(typeName=="sys")
                        {
                            typeName = "System";
                            if (itemName.endsWith("FSB")) units="mhz";
                            if (itemName.endsWith("Clock")) units="mhz";
                            if (itemName.endsWith("Utilization")) units="%";
                            if (itemName.endsWith("Memory")) units="MB";
                        }
                        if(typeName=="temp")
                        {
                            typeName = "Temperatures";
                            units=QLatin1Literal("\260C");
                        }
                        if(typeName=="fan")
                        {
                            typeName = "Cooling Fans";
                            units="rpm";
                        }
                        if(typeName=="duty")
                        {
                            typeName = "Fan Speeds";
                        }
                        if(typeName=="volt")
                        {
                            typeName = "Voltage Values";
                            units="V";
                        }

                        bool ok;
                        float valueFloat = valueString.toFloat(&ok);
                        if(ok)
                            updateValue(typeName,groupName,itemName, valueFloat, SensorDefinition(units));
                        else
                            updateValue(typeName,groupName,itemName, valueString, SensorDefinition(units));
                    }
                }
                n = n.nextSibling();
            }


            UnmapViewOfFile(aidaData);
        }
        else
            resultVal = false;
        CloseHandle(filemap);
    }
    else
        resultVal = false;

    return resultVal;
#else
    return false;
#endif
}
