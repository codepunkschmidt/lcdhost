#include "LH_MonitoringSource_HWiNFO.h"

LH_MonitoringSource_HWiNFO::LH_MonitoringSource_HWiNFO(LH_QtObject *parent): LH_MonitoringSource(parent, "HWiNFO", true) {}

bool LH_MonitoringSource_HWiNFO::doUpdate()
{
#ifndef Q_WS_WIN
    return false;
#else
    bool resultVal = false;
    const char* mapname  = HWiNFO_SENSORS_MAP_FILE_NAME;

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    setDataAvailable(filemap != NULL);
    if(dataAvailable())
    {
        HWiNFO_SENSORS_SHARED_MEM* hwinfoMemory = (HWiNFO_SENSORS_SHARED_MEM*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(HWiNFO_SENSORS_SHARED_MEM));

        if (hwinfoMemory) {
            if(hwinfoMemory->header.dwVersion!=3)
            {
                qWarning() << "LH_Monitoring: HWiNFO version is incompatible.";
                return false;
            }

            for (uint i=0; i<HWiNFO_MAX_SENSORS; i++)
            {
                HWiNFO_SENSORS_SENSOR_ENTRY hwinfoSensor = hwinfoMemory->Sensors[i];
                QString sensorName = QString(hwinfoSensor.szSensorName);
                if( QString(hwinfoSensor.szSensorName) == "" || !hwinfoSensor.fValid)
                    break;
                else
                {
                    updateReadingListValues(sensorName, "Temperatures", hwinfoSensor.Temperatures, HWiNFO_MAX_TEMPERATURES, true);
                    updateReadingListValues(sensorName, "Voltages", hwinfoSensor.Voltages, HWiNFO_MAX_VOLTAGES);
                    updateReadingListValues(sensorName, "Fans", hwinfoSensor.Fans, HWiNFO_MAX_FANS);
                    updateReadingListValues(sensorName, "Currents", hwinfoSensor.Currents, HWiNFO_MAX_CURRENTS);
                    updateReadingListValues(sensorName, "Powers", hwinfoSensor.Powers, HWiNFO_MAX_POWERS);
                    updateReadingListValues(sensorName, "Clocks", hwinfoSensor.Clocks, HWiNFO_MAX_CLOCKS);
                    updateReadingListValues(sensorName, "Usages", hwinfoSensor.Usages, HWiNFO_MAX_USAGES, true);
                    updateReadingListValues(sensorName, "Others", hwinfoSensor.Others, HWiNFO_MAX_OTHER);
                }
            }

            resultVal = true;
            UnmapViewOfFile(hwinfoMemory);
        }
        CloseHandle(filemap);
    }

    return resultVal;
#endif
}

void LH_MonitoringSource_HWiNFO::updateReadingListValues(QString sensorName, QString typeName, HWiNFO_SENSORS_READING_LIST* items, int length, bool aggregate)
{
    for(int j=0; j<length; j++)
    {
        if( items[j].fValid )
        {
            QVariant val = items[j].Value;
            if(QString(items[j].szUnit) == "Yes/No")
                updateValue(typeName, sensorName, QString(items[j].szLabel), (val == 0? "No": "Yes") );
            else
                updateValue(typeName, sensorName, QString(items[j].szLabel), val.toFloat(), SensorDefinition( items[j].szUnit ));
        }
    }
    if(aggregate)
        updateAggregates(typeName, sensorName);
}
