#include "LH_GPUZData.h"

LH_GPUZData::LH_GPUZData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    ui_->reset();
    ui_->setVisible(mon_type);
    ui_->refresh(mon_type);
}

LH_GPUZData::~LH_GPUZData()
{

}

monitoringDataType LH_GPUZData::getType()
{
    QString units = getUnits();

    if(units == "MHz")
        return mdtInt;
    if(units == "°C")
        return mdtInt;
    if(units == "%")
        return mdtFloat;
    if(units == "RPM")
        return mdtFloat;
    if(units == "V")
        return mdtFloat;

    return mdtInt;
}

bool LH_GPUZData::getData(float& value, QString& text, QString& units)
{
#ifdef Q_WS_WIN
    const char* mapname  = "GPUZShMem";
    float resultVal = true;

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    // Get pointer
    if(filemap != NULL)
    {
        GPUZ_SH_MEM* gpuzMemory = (GPUZ_SH_MEM*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(GPUZ_SH_MEM));

        if (ui_->count(mon_type) == 0) loadTypesList(gpuzMemory);

        if (gpuzMemory) {
            if(gpuzMemory->version!=0)
                getSelectedValue(gpuzMemory, value, text, units);
            UnmapViewOfFile(gpuzMemory);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    ui_->setReadonly(mon_type, !resultVal);
    return resultVal;
#else
    Q_UNUSED(value);
    Q_UNUSED(text);
    Q_UNUSED(units);
    return false;
#endif
}

void LH_GPUZData::getSelectedValue(GPUZ_SH_MEM* gpuzMemory, float& value, QString& text, QString& units)
{
    int index = ui_->setup_value_type_index_->value();

    value = gpuzMemory->sensors[index].value;
    text = "";
    units = QString::fromWCharArray(gpuzMemory->sensors[index].unit);
    if(units=="%%") units = "%";

    return ;
}

void LH_GPUZData::loadTypesList(GPUZ_SH_MEM* gpuzMemory)
{
    ui_->clear(mon_type);
    int i=0;
    while (i<256)
    {
        QString sensorName = QString::fromWCharArray( gpuzMemory->sensors[i++].name );
        if( sensorName == "" )
            break;
        else
            ui_->append( mon_type, sensorName );
    }
    ui_->refresh(mon_type);
    ui_->setIndex(mon_type, ui_->setup_value_type_index_->value());
}
