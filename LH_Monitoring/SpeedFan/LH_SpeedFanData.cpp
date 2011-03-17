#include "LH_SpeedFanData.h"

LH_SpeedFanData::LH_SpeedFanData(LH_QtObject *parent, bool includeGroups): LH_MonitoringData( parent, includeGroups )
{
    QStringList valueTypes = QStringList();
    valueTypes.append("Temperature");
    valueTypes.append("Fan Speed");
    valueTypes.append("Voltage");

    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Type", valueTypes, LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Type)",0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_type_index_->setOrder(-4);

    setup_value_index_ = new LH_Qt_QStringList(parent,"Value Item", QStringList(), LH_FLAG_READONLY);
    setup_value_index_->setHelp( "<p>The value to display.</p>");
    setup_value_index_->setOrder(-4);

    setup_value_index_index_ = new LH_Qt_int(parent,"(Value Item)",0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_index_index_->setOrder(-4);

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_type_index_, SIGNAL(changed()), parent, SLOT(setTypeSelection()) );
    parent->connect(setup_value_index_, SIGNAL(changed()), parent, SLOT(changeItemSelection()) );
    parent->connect(setup_value_index_index_, SIGNAL(changed()), parent, SLOT(setItemSelection()) );
}

LH_SpeedFanData::~LH_SpeedFanData()
{

}

monitoringDataType LH_SpeedFanData::getType()
{
    if(is_avg_)
        return mdtFloat;
    else
        return (setup_value_type_->value()!=2? mdtInt : mdtFloat);
}

int LH_SpeedFanData::getThreshMax()
{
    return (setup_value_type_->value()==2? 12 : (setup_value_type_->value()==1? 99999 : 200));
}

bool LH_SpeedFanData::getData(float& value, QString& text, QString& units)
{
    int count;
    return getData(value, text, units, count, setup_value_index_->value());
}

bool LH_SpeedFanData::getData(float& value, QString& text, QString& units, int index)
{
    int count;
    return getData(value, text, units, count, index);
}

bool LH_SpeedFanData::getData(float& value, QString& text, QString& units, int& count, int index)
{
#ifdef Q_WS_WIN
    const char* mapname  = "SFSharedMemory_ALM";
    bool resultVal = true;

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    // Get pointer
    if(filemap != NULL)
    {
        SFMemory* sfmemory = (SFMemory*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, sizeof(SFMemory));

        if (sfmemory) {
            if(sfmemory->MemSize!=0) {
                if (setup_value_index_->list().length() == 0)
                    loadSensorList(sfmemory);
                getSelectedValue(sfmemory, value, text, units, count, index);
            }
            UnmapViewOfFile(sfmemory);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    setup_value_index_->setFlag(LH_FLAG_READONLY, !resultVal);
    return resultVal;
#endif
    return false;
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
        switch (setup_value_type_->value())
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
    switch (setup_value_type_->value())
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
    if (setup_value_type_->value() == 0) value /= 100.0 ;
    return result;
}

void LH_SpeedFanData::getSelectedValue(SFMemory* sfmemory, float& value, QString& text, QString& units, int& count, int index)
{
    QString type;

    text = "";
    count = 0;

    switch (setup_value_type_->value())
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
    is_avg_ = (setup_value_index_->value()==sel_id_avg_);
    is_group_ = (setup_value_index_->value()==sel_id_all_);

    return ;
}

void LH_SpeedFanData::loadSensorList(SFMemory* sfmemory)
{
    setup_value_index_->list().clear();
    int count = 0;
    QString type;
    if(sfmemory)
    {
        switch (setup_value_type_->value())
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
            setup_value_index_->list().append( type + " #" + QString::number(i+1) );

        sel_id_all_ = -2;
        sel_id_avg_ = -2;
        if(setup_value_type_->value()!=2 && count>1)
        {
            setup_value_index_->list().append( "Average " + type );
            sel_id_avg_ = setup_value_index_->list().count()-1;
            setup_value_index_->list().append( "Lowest " + type );
            setup_value_index_->list().append( "Highest " + type );
            if(includeGroups_)
            {
                setup_value_index_->list().append( "All " + type + "s" );
                sel_id_all_ = setup_value_index_->list().count()-1;
            }
        }
    }

    setup_value_index_->refreshList();
    setup_value_index_->setValue(setup_value_index_index_->value());
}

void LH_SpeedFanData::setTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_SpeedFanData::setItemSelection()
{
    if (setup_value_index_->list().length()!=0)
        setup_value_index_->setValue(setup_value_index_index_->value());
}

void LH_SpeedFanData::changeTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_index_->setValue(setup_value_type_->value());

    setup_value_index_->list().clear();
}

void LH_SpeedFanData::changeItemSelection()
{
    if (setup_value_index_->list().length()!=0)
        setup_value_index_index_->setValue(setup_value_index_->value());

    float value;
    QString units;
    QString text;
    getData(value, text, units);
}
