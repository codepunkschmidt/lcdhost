#include "LH_MonitoringSource_SpeedFan.h"
#include "LH_MonitoringTypes_SpeedFan.h"

#ifdef Q_OS_WIN
# include <windows.h>
#endif

LH_MonitoringSource_SpeedFan::LH_MonitoringSource_SpeedFan(LH_QtObject *parent): LH_MonitoringSource(parent, "SpeedFan") {
    temp_ = SensorDefinition( "\xB0C", -128.0 );
    fans_ = SensorDefinition( "rpm", 0 );
    volt_ = SensorDefinition( "V" );
}


bool LH_MonitoringSource_SpeedFan::doUpdate()
{
#ifndef Q_OS_WIN
    return false;
#else
    bool resultVal = false;

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, "SFSharedMemory_ALM");

    setDataAvailable(filemap != NULL);
    if(dataAvailable())
    {
        if (SFMemory* sfmemory = (SFMemory*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(SFMemory)))
        {
            if(sfmemory->MemSize!=0)
            {
                updateFromArray("Temperature", sfmemory->NumTemps, sfmemory->temps, 0.01, temp_);
                updateFromArray("Fan",         sfmemory->NumFans,  sfmemory->fans,  1.00, fans_);
                updateFromArray("Voltage",     sfmemory->NumVolts, sfmemory->volts, 0.01, volt_);
                resultVal = true;
            }
            UnmapViewOfFile(sfmemory);
        }
        CloseHandle(filemap);
    }
    return resultVal;
#endif
}

void LH_MonitoringSource_SpeedFan::updateFromArray(QString type, int count, const int *valueSet, float modifier, SensorDefinition def)
{
    if (count == 0)
        return;
    for(int i = 0; i<count; i++)
        updateValue(type, "", QString("%1 #%2").arg(type).arg(i+1), (modifier * (float)valueSet[i]), def);
    updateAggregates(type, "");
}

void LH_MonitoringSource_SpeedFan::updateFromArray(QString type, int count, const uint *valueSet, float modifier, SensorDefinition def)
{
    if (count == 0)
        return;
    for(int i = 0; i<count; i++)
        updateValue(type, "", QString("%1 #%2").arg(type).arg(i+1), (modifier * (float)valueSet[i]), def);
    updateAggregates(type, "");
}

void LH_MonitoringSource_SpeedFan::updateFromArray(QString type, int count, const float *valueSet, float modifier, SensorDefinition def)
{
    if (count == 0)
        return;
    for(int i = 0; i<count; i++)
        updateValue(type, "", QString("%1 #%2").arg(type).arg(i+1), (modifier * (float)valueSet[i]), def);
    updateAggregates(type, "");
}
