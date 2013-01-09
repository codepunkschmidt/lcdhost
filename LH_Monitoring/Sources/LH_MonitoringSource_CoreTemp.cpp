#include "LH_MonitoringSource_CoreTemp.h"
#include "LH_MonitoringTypes_CoreTemp.h"

LH_MonitoringSource_CoreTemp::LH_MonitoringSource_CoreTemp(LH_QtObject *parent): LH_MonitoringSource(parent, "CoreTemp")
{

    temp_ = SensorDefinition( "\176?" );
    freq_ = SensorDefinition( "MHz" );
    toTj_ = SensorDefinition( "\176? to TjMax" );
    perc_ = SensorDefinition( "%", minmax(OptionalValue(true, 0), OptionalValue(true, 100)));

}

bool LH_MonitoringSource_CoreTemp::doUpdate()
{
#ifndef Q_WS_WIN
    return false;
#else
    const char* mapname  = "CoreTempMappingObject";
    bool resultVal = false;

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    setDataAvailable(filemap != NULL);
    if(dataAvailable())
    {
        CTMemory* ctmemory = (CTMemory*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(CTMemory));

        if (ctmemory) {
            if( !((ctmemory->uiCPUCnt==0) && (ctmemory->uiCoreCnt==0)) )
            {
                resultVal = true;
                temp_.units = (ctmemory->ucFahrenheit? "\176F" : "\176C");
                toTj_.units = temp_.units +  " to TjMax";

                updateFromArray("Core Temperature", "Core", ctmemory->uiCoreCnt, ctmemory->fTemp  , (ctmemory->ucDeltaToTjMax? toTj_ : temp_));
                updateFromArray("Core Load"       , "Core", ctmemory->uiCoreCnt, ctmemory->uiLoad , perc_);
                updateFromArray("CPU TjMax"       , "CPU" , ctmemory->uiCPUCnt , ctmemory->uiTjMax, temp_);

                updateValue("CPU Count","","CPU Count",ctmemory->uiCPUCnt);
                updateValue("Core Count","","Core Count",ctmemory->uiCoreCnt);
                updateValue("Voltage Identification","","Voltage Identification",ctmemory->fVID);
                updateValue("CPU Speed","","CPU Speed",ctmemory->fCPUSpeed, freq_);
                updateValue("FSB Speed","","FSB Speed",ctmemory->fFSBSpeed, freq_);
                updateValue("Multiplier","","Multiplier",ctmemory->fMultiplier);
                updateValue("CPU Name","","CPU Name",ctmemory->sCPUName);
            }
            UnmapViewOfFile(ctmemory);
        }
        CloseHandle(filemap);
    }
    return resultVal;
#endif
}

void LH_MonitoringSource_CoreTemp::updateFromArray(QString type, QString name, int count, const uint *valueSet, SensorDefinition def)
{
    if (count == 0)
        return;
    for(int i = 0; i<count; i++)
        updateValue(type, "", QString("%1 #%2").arg(name).arg(i+1), (float)valueSet[i], def);
    updateAggregates(type, "");
}

void LH_MonitoringSource_CoreTemp::updateFromArray(QString type, QString name, int count, const float *valueSet, SensorDefinition def)
{
    if (count == 0)
        return;
    for(int i = 0; i<count; i++)
        updateValue(type, "", QString("%1 #%2").arg(name).arg(i+1), (float)valueSet[i], def);
    updateAggregates(type, "");
}
