#include "LH_CoreTempData.h"

LH_CoreTempData::LH_CoreTempData(LH_QtObject *parent, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, includeGroups )
{
    valueTypes = QStringList();
    if(dataMode & mdmNumbers) valueTypes.append("Core Temperature");
    if(dataMode & mdmText) valueTypes.append("Core Load");
    if(dataMode & mdmText) valueTypes.append("CPU TjMax");
    if(dataMode & mdmText) valueTypes.append("CPU Count");
    if(dataMode & mdmText) valueTypes.append("Core Count");
    if(dataMode & mdmNumbers) valueTypes.append("Voltage Identification");
    if(dataMode & mdmNumbers) valueTypes.append("CPU Speed");
    if(dataMode & mdmNumbers) valueTypes.append("FSB Speed");
    if(dataMode & mdmNumbers) valueTypes.append("Multiplier");
    if(dataMode & mdmText) valueTypes.append("CPU Name");

    setup_value_type_ = new LH_Qt_QStringList(parent, "Value Type", valueTypes, LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Type)",0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_type_index_->setOrder(-4);

    setup_value_index_ = new LH_Qt_QStringList(parent,"Value Item", QStringList(), LH_FLAG_READONLY);
    setup_value_index_->setHelp( "<p>The value to display.</p>");
    setup_value_index_->setOrder(-4);

    setup_value_index_index_ = new LH_Qt_int(parent,"(Value Item)",0,LH_FLAG_HIDDEN);
    setup_value_index_index_->setOrder(-4);


    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_type_index_, SIGNAL(changed()), parent, SLOT(setTypeSelection()) );
    parent->connect(setup_value_index_, SIGNAL(changed()), parent, SLOT(changeItemSelection()) );
    parent->connect(setup_value_index_index_, SIGNAL(changed()), parent, SLOT(setItemSelection()) );

}

LH_CoreTempData::~LH_CoreTempData()
{

}


monitoringDataType LH_CoreTempData::getType()
{
    if(is_avg_)
        return mdtFloat;

    if(setup_value_type_->list().at(setup_value_type_->value()) == "Core Temperature")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Core Load")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU TjMax")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU Count")
        return mdtText;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Core Count")
        return mdtText;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Voltage Identification")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU Speed")
        return mdtFloat;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "FSB Speed")
        return mdtFloat;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Multiplier")
        return mdtInt;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU Name")
        return mdtText;

    return mdtInt;
}

int LH_CoreTempData::getThreshMax()
{
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Core Temperature")
        return 200;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Core Load")
        return 100;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU TjMax")
        return 200;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Voltage Identification")
        return 4096;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU Speed")
        return 99999;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "FSB Speed")
        return 99999;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "Multiplier")
        return 99999;
    if(setup_value_type_->list().at(setup_value_type_->value()) == "CPU Name")
        return 0;

    return 99999;
}

bool LH_CoreTempData::getData(float& value, QString& text, QString& units)
{
    int count;
    return getData(value, text, units, count, setup_value_index_->value());
}
bool LH_CoreTempData::getData(float& value, QString& text, QString& units, int index)
{
    int count;
    return getData(value, text, units, count, index);
}
bool LH_CoreTempData::getData(float& value, QString& text, QString& units, int& count, int index)
{
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

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    setup_value_index_->setFlag(LH_FLAG_READONLY, !resultVal);
    return resultVal;
}

bool LH_CoreTempData::getCount(int& count)
{
    float value; QString text; QString units;
    count = 0;
    return getData(value, text, units, count, 0);
}

bool LH_CoreTempData::getNames(QStringList& names)
{
    QString type = valueTypes.at(setup_value_type_->value());
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
    QString type = valueTypes.at(setup_value_type_->value());

    units = "";
    text = "";
    value = 0;
    count = 1;

    if(type=="Core Temperature") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, false);
        count = ctmemory->uiCoreCnt;
        units = (ctmemory->ucFahrenheit? "°F" : "°C");
        if (ctmemory->ucDeltaToTjMax) units += " to TjMax";
        loadSensorList(count, "Core");
        value = getValueFromSet(index, count, ctmemory->fTemp);
    }
    if(type=="Core Load") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, false);
        count = ctmemory->uiCoreCnt;
        units = "%";
        loadSensorList(count, "Core");
        value = getValueFromSet(index, count, ctmemory->uiLoad);
    }
    if(type=="CPU TjMax") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, false);
        count = ctmemory->uiCPUCnt;
        units = (ctmemory->ucFahrenheit? "°F" : "°C");
        loadSensorList(count, "CPU");
        value = getValueFromSet(index, count, ctmemory->uiTjMax);
    }

    if(type=="CPU Count"){
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        value = ctmemory->uiCPUCnt;
    }
    if(type=="Core Count"){
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        value = ctmemory->uiCoreCnt;
    }

    if(type=="Voltage Identification") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        value = ctmemory->fVID;
    }
    if(type=="CPU Speed") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        value = ctmemory->fCPUSpeed;
        units = "MHz";
    }
    if(type=="FSB Speed") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        value = ctmemory->fFSBSpeed;
        units = "MHz";
    }
    if(type=="Multiplier") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        value = ctmemory->fMultiplier;
    }
    if(type=="CPU Name") {
        setup_value_index_->setFlag(LH_FLAG_HIDDEN, true);
        text = ctmemory->sCPUName;
    }

    //really a bad place to put these but...
    if(is_avg_!=-2)     is_avg_ = (setup_value_index_->value()==sel_id_avg_);
    if(sel_id_all_!=-2) is_group_ = (setup_value_index_->value()==sel_id_all_);

    return ;
}

void LH_CoreTempData::loadSensorList(int count, QString type)
{
    setup_value_index_->list().clear();
    for(int i=0; i<count; i++)
        setup_value_index_->list().append( type + " #" + QString::number(i+1) );

    sel_id_all_ = -2;
    sel_id_avg_ = -2;
    if(count>1)
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

    setup_value_index_->refreshList();
    setup_value_index_->setValue(setup_value_index_index_->value());
}

void LH_CoreTempData::setTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_CoreTempData::setItemSelection()
{
    if (setup_value_index_->list().length()!=0)
        setup_value_index_->setValue(setup_value_index_index_->value());
}

void LH_CoreTempData::changeTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_index_->setValue(setup_value_type_->value());

    setup_value_index_->list().clear();
}

void LH_CoreTempData::changeItemSelection()
{
    if (setup_value_index_->list().length()!=0)
        setup_value_index_index_->setValue(setup_value_index_->value());

    float value;
    QString units;
    QString text;
    getData(value, text, units);
}
