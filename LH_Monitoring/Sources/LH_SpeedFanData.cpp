#include <QSharedMemory>
#include "LH_SpeedFanData.h"

#ifdef Q_WS_WIN
# include <windows.h>
#endif

LH_SpeedFanData::LH_SpeedFanData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups): LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    Q_UNUSED(dataMode);

    ui_->reset();
    ui_->setVisible(mon_type | mon_item);

    ui_->append(mon_type, "Temperature");
    ui_->append(mon_type, "Fan Speed");
    ui_->append(mon_type, "Voltage");

    ui_->refresh(mon_type);
}

monitoringDataType LH_SpeedFanData::getType()
{
    if(is_avg_)
        return mdtFloat;
    else
        return (ui_->value(mon_type)!=2? mdtInt : mdtFloat);
}

int LH_SpeedFanData::getThreshMax()
{
    return (ui_->value(mon_type)==2? 12 : (ui_->value(mon_type)==1? 99999 : 200));
}

bool LH_SpeedFanData::getData(float& value, QString& text, QString& units)
{
    int count;
    return getData(value, text, units, count, ui_->value(mon_item));
}

bool LH_SpeedFanData::getData(float& value, QString& text, QString& units, int index)
{
    int count;
    return getData(value, text, units, count, index);
}

bool LH_SpeedFanData::getData(float& value, QString& text, QString& units, int& count, int index)
{
#ifdef Q_WS_WIN
    bool resultVal = false;

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, "SFSharedMemory_ALM");
    if(filemap != NULL)
    {
        SFMemory* sfmemory = (SFMemory*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(SFMemory));
        if (sfmemory)
        {
            if(sfmemory->MemSize!=0)
            {
                if (ui_->count(mon_item) == 0)
                    loadSensorList(sfmemory);
                getSelectedValue(sfmemory, value, text, units, count, index);
                resultVal = true;
            }
            UnmapViewOfFile(sfmemory);
        }
        CloseHandle(filemap);
    }
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

bool LH_SpeedFanData::getCount(int& count)
{
    float value; QString text; QString units;
    count = 1;
    if(is_group_)
        return getData(value, text, units, count, 0);
    else
        return true;
}

bool LH_SpeedFanData::getNames(QStringList& names)
{
    int count;
    if(!getCount(count)) count = 1;

    names.clear();
    if(!is_group_ || count==1)
        names.append("Default");
    else
    {
        QString type = "";
        switch (ui_->value(mon_type))
        {
        case 0:
            type = "Temperature";
            break;
        case 1:
            type = "Fan Speed";
            break;
        case 2:
            type = "Voltage";
            break;
        }
        for(int i=0; i<count; i++)
            names.append( QString("%1 #%2").arg(type).arg(QString::number(i+1)) );
    }
    return true;
}

bool LH_SpeedFanData::getDeadValue(float& value)
{
    switch (ui_->value(mon_type))
    {
    case 0:
        value = -12800;
        return true;
    case 1:
        value = 0;
        return true;
    default:
        return false;
    }
}
bool LH_SpeedFanData::getDeadValue_Transformed(float& value)
{
    bool result = getDeadValue(value);
    if (ui_->value(mon_type) == 0) value /= 100.0 ;
    return result;
}

void LH_SpeedFanData::getSelectedValue(const SFMemory* sfmemory, float& value, QString& text, QString& units, int& count, int index)
{
    QString type;

    text = "";
    count = 0;

    switch (ui_->value(mon_type))
    {
    case 0:
        type = "Temperature";
        count = sfmemory->NumTemps;
        value = getValueFromSet(index, count, sfmemory->temps)/100.0; //speedfan reports disconnected temp sensors @-128
        units = "°C";
        break;
    case 1:
        type = "Fan Speed";
        count = sfmemory->NumFans;
        value = getValueFromSet(index, count, sfmemory->fans); //speedfan reports disconnected fan sensors @0
        units = "rpm";
        break;
    case 2:
        type = "Voltage";
        count = sfmemory->NumVolts;
        if(index>=count && count>0) index = count - 1;
        value = float(sfmemory->volts[index])/100.0;
        units = "V";
        break;
    }

    //really a bad place to put these but...
    is_avg_ = (ui_->value(mon_item)==sel_id_avg_);
    is_group_ = (ui_->value(mon_item)==sel_id_all_);

    return ;
}

void LH_SpeedFanData::loadSensorList(const SFMemory* sfmemory)
{
    ui_->clear(mon_item);
    int count = 0;
    QString type;
    if(sfmemory)
    {
        switch (ui_->value(mon_type))
        {
        case 0:
            type = "Temperature";
            count = sfmemory->NumTemps;
            break;
        case 1:
            type = "Fan";
            count = sfmemory->NumFans;
            break;
        case 2:
            type = "Voltage";
            count = sfmemory->NumVolts;
            break;
        }

        for(int i=0; i<count; i++)
            ui_->append( mon_item, type + " #" + QString::number(i+1) );

        sel_id_all_ = -2;
        sel_id_avg_ = -2;
        if(ui_->value(mon_type)!=2 && count>1)
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
    }

    ui_->refresh(mon_item);
    ui_->setValue(mon_item, ui_->setup_value_item_index_->value());
}
