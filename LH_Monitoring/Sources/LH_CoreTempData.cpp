#include "LH_CoreTempData.h"

LH_CoreTempData::LH_CoreTempData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups ) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    ui_->reset();
    ui_->setVisible(mon_type | mon_item);

    if(dataMode & mdmNumbers) ui_->append(mon_type, "Core Temperature");
    if(dataMode & mdmText) ui_->append(mon_type, "Core Load");
    if(dataMode & mdmText) ui_->append(mon_type, "CPU TjMax");
    if(dataMode & mdmText) ui_->append(mon_type, "CPU Count");
    if(dataMode & mdmText) ui_->append(mon_type, "Core Count");
    if(dataMode & mdmNumbers) ui_->append(mon_type, "Voltage Identification");
    if(dataMode & mdmNumbers) ui_->append(mon_type, "CPU Speed");
    if(dataMode & mdmNumbers) ui_->append(mon_type, "FSB Speed");
    if(dataMode & mdmNumbers) ui_->append(mon_type, "Multiplier");
    if(dataMode & mdmText) ui_->append(mon_type, "CPU Name");

    ui_->refresh(mon_type);
}

monitoringDataType LH_CoreTempData::getType()
{
    if(is_avg_)
        return mdtFloat;

    if(ui_->valueText(mon_type) == "Core Temperature")
        return mdtInt;
    if(ui_->valueText(mon_type) == "Core Load")
        return mdtInt;
    if(ui_->valueText(mon_type) == "CPU TjMax")
        return mdtInt;
    if(ui_->valueText(mon_type) == "CPU Count")
        return mdtText;
    if(ui_->valueText(mon_type) == "Core Count")
        return mdtText;
    if(ui_->valueText(mon_type) == "Voltage Identification")
        return mdtInt;
    if(ui_->valueText(mon_type) == "CPU Speed")
        return mdtFloat;
    if(ui_->valueText(mon_type) == "FSB Speed")
        return mdtFloat;
    if(ui_->valueText(mon_type) == "Multiplier")
        return mdtInt;
    if(ui_->valueText(mon_type) == "CPU Name")
        return mdtText;

    return mdtInt;
}

int LH_CoreTempData::getThreshMax()
{
    if(ui_->valueText(mon_type) == "Core Temperature")
        return 200;
    if(ui_->valueText(mon_type) == "Core Load")
        return 100;
    if(ui_->valueText(mon_type) == "CPU TjMax")
        return 200;
    if(ui_->valueText(mon_type) == "Voltage Identification")
        return 4096;
    if(ui_->valueText(mon_type) == "CPU Speed")
        return 99999;
    if(ui_->valueText(mon_type) == "FSB Speed")
        return 99999;
    if(ui_->valueText(mon_type) == "Multiplier")
        return 99999;
    if(ui_->valueText(mon_type) == "CPU Name")
        return 0;

    return 99999;
}

bool LH_CoreTempData::getData(float& value, QString& text, QString& units)
{
    int count;
    return getData(value, text, units, count, ui_->value(mon_item));
}
bool LH_CoreTempData::getData(float& value, QString& text, QString& units, int index)
{
    int count;
    return getData(value, text, units, count, index);
}

bool LH_CoreTempData::getData(float& value, QString& text, QString& units, int& count, int index)
{
#ifdef Q_WS_WIN
    const char* mapname  = "CoreTempMappingObject";
    bool resultVal = true;

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    // Get pointer
    if(filemap != NULL)
    {
        CTMemory* ctmemory = (CTMemory*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(CTMemory));

        if (ctmemory) {
            if( !((ctmemory->uiCPUCnt==0) && (ctmemory->uiCoreCnt==0)) )
                getSelectedValue(ctmemory, value, text, units, count, index);
            UnmapViewOfFile(ctmemory);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    ui_->setReadonly(mon_type | mon_item, !resultVal);
    return resultVal;
#else
    Q_UNUSED(value);
    Q_UNUSED(text);
    Q_UNUSED(units);
    Q_UNUSED(count);
    Q_UNUSED(index);
    return false;
#endif
}

bool LH_CoreTempData::getCount(int& count)
{
    float value; QString text; QString units;
    count = 0;
    return getData(value, text, units, count, 0);
}

bool LH_CoreTempData::getNames(QStringList& names)
{
    QString type = ui_->valueText(mon_type);
    names.clear();
    int count;
    if (getCount(count) && count>1)
    {
        if(type=="Core Temperature" || type=="Core Load") {
            for(int i=0; i<count; i++)
                names.append( QString("Core #%1").arg(QString::number(i+1)) );
            return true;
        }
        if(type=="CPU TjMax") {
            for(int i=0; i<count; i++)
                names.append( QString("CPU #%1").arg(QString::number(i+1)) );
            return true;
        }
    }

    names.append("Default");
    return true;
}

void LH_CoreTempData::getSelectedValue(CTMemory* ctmemory, float& value, QString& text, QString& units, int& count, int index)
{
    QString type = ui_->valueText(mon_type);

    units = "";
    text = "";
    value = 0;
    count = 1;

    if(type=="Core Temperature") {
        ui_->setVisible(mon_item, true);
        count = ctmemory->uiCoreCnt;
        units = (ctmemory->ucFahrenheit? "°F" : "°C");
        if (ctmemory->ucDeltaToTjMax) units += " to TjMax";
        if(ui_->count(mon_item) == 0)
                loadSensorList(count, "Core");
        value = getValueFromSet(index, count, ctmemory->fTemp);
    }
    if(type=="Core Load") {
        ui_->setVisible(mon_item, true);
        count = ctmemory->uiCoreCnt;
        units = "%";
        if(ui_->count(mon_item) == 0)
                loadSensorList(count, "Core");
        value = getValueFromSet(index, count, ctmemory->uiLoad);
    }
    if(type=="CPU TjMax") {
        ui_->setVisible(mon_item, true);
        count = ctmemory->uiCPUCnt;
        units = (ctmemory->ucFahrenheit? "°F" : "°C");
        if(ui_->count(mon_item) == 0)
            loadSensorList(count, "CPU");
        value = getValueFromSet(index, count, ctmemory->uiTjMax);
    }

    if(type=="CPU Count"){
        ui_->setVisible(mon_item, false);
        value = ctmemory->uiCPUCnt;
    }
    if(type=="Core Count"){
        ui_->setVisible(mon_item, false);
        value = ctmemory->uiCoreCnt;
    }

    if(type=="Voltage Identification") {
        ui_->setVisible(mon_item, false);
        value = ctmemory->fVID;
    }
    if(type=="CPU Speed") {
        ui_->setVisible(mon_item, false);
        value = ctmemory->fCPUSpeed;
        units = "MHz";
    }
    if(type=="FSB Speed") {
        ui_->setVisible(mon_item, false);
        value = ctmemory->fFSBSpeed;
        units = "MHz";
    }
    if(type=="Multiplier") {
        ui_->setVisible(mon_item, false);
        value = ctmemory->fMultiplier;
    }
    if(type=="CPU Name") {
        ui_->setVisible(mon_item, false);
        text = ctmemory->sCPUName;
    }

    //really a bad place to put these but...
    if(is_avg_!=-2)     is_avg_ = (ui_->value(mon_item)==sel_id_avg_);
    if(sel_id_all_!=-2) is_group_ = (ui_->value(mon_item)==sel_id_all_);

    return ;
}

void LH_CoreTempData::loadSensorList(int count, QString type)
{
    ui_->clear(mon_item);
    for(int i=0; i<count; i++)
        ui_->append( mon_item, type + " #" + QString::number(i+1) );

    sel_id_all_ = -2;
    sel_id_avg_ = -2;
    if(count>1)
    {
        ui_->append( mon_item, "Average " + type );
        sel_id_avg_ = ui_->count(mon_item)-1;

        ui_->append( mon_item, "Lowest " + type );
        ui_->append( mon_item, "Highest " + type );
        if(includeGroups_)
        {
            ui_->append( mon_item, "All " + type + "s" );
            sel_id_all_ = ui_->count(mon_item)-1;
        }
    }

    ui_->refresh(mon_item);
    ui_->setValue(mon_item, ui_->setup_value_item_index_->value());
}
