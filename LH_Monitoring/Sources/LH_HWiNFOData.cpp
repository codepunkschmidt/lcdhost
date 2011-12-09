#include "LH_HWiNFOData.h"

LH_HWiNFOData::LH_HWiNFOData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    ui_->reset();
    ui_->setVisible(mon_type | mon_group | mon_item);
    ui_->refresh(mon_type | mon_group | mon_item);

    listedType_ = -1;
    listedGroup_ = -1;
}

monitoringDataType LH_HWiNFOData::getType()
{
    return getType(getUnits());
}

monitoringDataType LH_HWiNFOData::getType(QString units)
{
    if(units == "Yes/No")
        return mdtText;
    if(units == "MHz")
        return mdtFloat;
    if(units == "°C")
        return mdtFloat;
    if(units == "%")
        return mdtFloat;
    if(units == "RPM")
        return mdtInt;
    if(units == "V")
        return mdtFloat;
    if(units == "A")
        return mdtFloat;

    return mdtInt;
}

bool LH_HWiNFOData::getData(float& value, QString& text, QString& units)
{
#ifdef Q_WS_WIN
    const char* mapname  = HWiNFO_SENSORS_MAP_FILE_NAME;
    float resultVal = true;

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    // Get pointer
    if(filemap != NULL)
    {
        _HWiNFO_SENSORS_SHARED_MEM* hwinfoMemory = (_HWiNFO_SENSORS_SHARED_MEM*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(_HWiNFO_SENSORS_SHARED_MEM));
        if (hwinfoMemory)
        {
            if(hwinfoMemory->header.dwVersion!=1)
            {
                qWarning() << "LH_Monitoring: HWiNFO version is incompatible.";
                return false;
            }

            if (ui_->count(mon_type) == 0)
                loadTypesList(hwinfoMemory);

            if(ui_->value(mon_type)!=-1)
            {
                if (ui_->value(mon_type) != listedType_)
                {
                    listedType_ = ui_->value(mon_type);
                    listedGroup_ = -1;
                    loadGroupsList(hwinfoMemory);
                }
                if(ui_->value(mon_group)!=-1)
                {
                    int max = -1;
                    HWiNFO_SENSORS_READING_LIST *list = getList(hwinfoMemory->Sensors[sensor_indexes_.value(ui_->value(mon_group))], max);

                    if (ui_->value(mon_group) != listedGroup_)
                    {
                        listedGroup_ = ui_->value(mon_group);
                        loadItemsList(list, max);
                    }

                    getSelectedValue(list, value, text, units);
                } else
                    resultVal = false;
            } else
                resultVal = false;
            UnmapViewOfFile(hwinfoMemory);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;
    return resultVal;
#else
    Q_UNUSED(value);
    Q_UNUSED(text);
    Q_UNUSED(units);
    return false;
#endif
}

QString LH_HWiNFOData::getText(bool addUnits, bool round)
{
    QString units;
    float value;
    QString resultText;
    if (getData(value, resultText, units))
    {
        if(round || getType()==mdtInt)
            value = int(value > 0.0 ? value + 0.5 : value - 0.5);
        if (resultText=="")
            resultText = QString("%1").arg(value);

        if(addUnits && units != "Yes/No") resultText = resultText + units;
    } else
        resultText = "N/A";

    return resultText;
}

HWiNFO_SENSORS_READING_LIST *LH_HWiNFOData::getList(HWiNFO_SENSORS_SENSOR_ENTRY hwinfoSensor, int &max )
{
    HWiNFO_SENSORS_READING_LIST *list = NULL;
    QString listName = ui_->valueText(mon_type);
    if(listName=="Temperatures") { list = hwinfoSensor.Temperatures; max = HWiNFO_MAX_TEMPERATURES; }
    if(listName=="Voltages") { list = hwinfoSensor.Voltages; max = HWiNFO_MAX_VOLTAGES; }
    if(listName=="Fans") { list = hwinfoSensor.Fans; max = HWiNFO_MAX_FANS; }
    if(listName=="Currents") { list = hwinfoSensor.Currents; max = HWiNFO_MAX_CURRENTS; }
    if(listName=="Powers") { list = hwinfoSensor.Powers; max = HWiNFO_MAX_POWERS; }
    if(listName=="Other") { list = hwinfoSensor.Others; max = HWiNFO_MAX_OTHER; }
    return list;
}

void LH_HWiNFOData::getSelectedValue(HWiNFO_SENSORS_READING_LIST *list, float& value, QString& text, QString& units)
{
    int index = ui_->value(mon_item);

    value = list[index].Value;
    text = QString::number(value);
    if(text.contains("e-"))
        text = QString::number(((int)(value*1000))/1000);
    units = QString( list[index].szUnit );
    if(units=="Yes/No")
        text = (value==1? "Yes" : "No");
    return ;
}

void LH_HWiNFOData::loadTypesList(_HWiNFO_SENSORS_SHARED_MEM* hwinfoMemory)
{
    ui_->clear(mon_type);
    int i=0;

    bool hasTemp = false;
    bool hasVolt = false;
    bool hasFans = false;
    bool hasCurr = false;
    bool hasPowr = false;
    bool hasOthr = false;

    while (i<HWiNFO_MAX_SENSORS)
    {
        HWiNFO_SENSORS_SENSOR_ENTRY hwinfoSensor = hwinfoMemory->Sensors[i++];
        if( QString(hwinfoSensor.szSensorName) == "" )
            break;
        else
        {
            if(!hasTemp) hasTemp = QString(hwinfoSensor.Temperatures[0].szLabel) != "";
            if(!hasVolt) hasVolt = QString(hwinfoSensor.Voltages[0].szLabel) != "";
            if(!hasFans) hasFans = QString(hwinfoSensor.Fans[0].szLabel) != "";
            if(!hasCurr) hasCurr = QString(hwinfoSensor.Currents[0].szLabel) != "";
            if(!hasPowr) hasPowr = QString(hwinfoSensor.Powers[0].szLabel) != "";
            if(!hasOthr) hasOthr = QString(hwinfoSensor.Others[0].szLabel) != "";
        }
    }
    if(hasTemp) ui_->append( mon_type, "Temperatures" );
    if(hasVolt) ui_->append( mon_type, "Voltages" );
    if(hasFans) ui_->append( mon_type, "Fans" );
    if(hasCurr) ui_->append( mon_type, "Currents" );
    if(hasPowr) ui_->append( mon_type, "Powers" );
    if(hasOthr) ui_->append( mon_type, "Other" );

    ui_->refresh(mon_type);
    ui_->setValue(mon_type, ui_->setup_value_type_index_->value());
    ui_->setReadonly(mon_type, ui_->count(mon_type)==0);
}

void LH_HWiNFOData::loadGroupsList(_HWiNFO_SENSORS_SHARED_MEM *hwinfoMemory)
{
    ui_->clear(mon_group);
    sensor_indexes_.clear();
    int i=0;
    int max;
    while (i<HWiNFO_MAX_SENSORS)
    {
        HWiNFO_SENSORS_SENSOR_ENTRY hwinfoSensor = hwinfoMemory->Sensors[i++];
        if( QString(hwinfoSensor.szSensorName) == "" )
            break;
        else
        {
            HWiNFO_SENSORS_READING_LIST *list = getList(hwinfoSensor, max);
            if(list == NULL)
                continue;
            int j = 0;
            while (j<max)
            {
                QString units = QString( list[j].szLabel);
                QString sensorName = QString( list[j++].szLabel );
                if (QString(sensorName) == "")
                    break;
                if(((dataMode_ & mdmText) == mdmText && getType(units) == mdtText) ||
                   ((dataMode_ & mdmNumbers) == mdmNumbers && getType(units) != mdtText))
                {
                    sensor_indexes_.insert(ui_->count(mon_group), i-1);
                    ui_->append( mon_group, QString(hwinfoSensor.szSensorName) );
                    break;
                }
            }
        }
    }
    ui_->refresh(mon_group);
    ui_->setValue(mon_group, ui_->setup_value_group_index_->value(), true);
    ui_->setReadonly(mon_group, ui_->count(mon_group)==0);
}

void LH_HWiNFOData::loadItemsList(HWiNFO_SENSORS_READING_LIST *list, int max)
{
    ui_->clear(mon_item);
    item_indexes_.clear();
    if(max!=-1)
    {
        int i=0;
        while (i<max)
        {
            QString units = QString( list[i].szLabel);
            QString sensorName = QString( list[i++].szLabel );
            if( sensorName == "" )
                break;
            else
            {
                if(((dataMode_ & mdmText) == mdmText && getType(units) == mdtText) ||
                   ((dataMode_ & mdmNumbers) == mdmNumbers && getType(units) != mdtText))
                {
                    item_indexes_.insert(ui_->count(mon_item), i);
                    ui_->append( mon_item, sensorName );
                }
            }
        }
    }
    ui_->refresh(mon_item);
    ui_->setValue(mon_item, ui_->setup_value_item_index_->value(), true);
    ui_->setReadonly(mon_item, ui_->count(mon_item)==0);
}
