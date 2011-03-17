#include "LH_GPUZData.h"

LH_GPUZData::LH_GPUZData(LH_QtObject *parent) : LH_MonitoringData( parent )
{
    setup_value_type_ = new LH_Qt_QStringList(parent,"Value Item", QStringList(), LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_type_index_ = new LH_Qt_int(parent,"(Value Item)", 0, LH_FLAG_HIDDEN);
    setup_value_type_index_->setOrder(-4);

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
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

        if (setup_value_type_->list().length() == 0) loadTypesList(gpuzMemory);

        if (gpuzMemory) {
            if(gpuzMemory->version!=0)
                getSelectedValue(gpuzMemory, value, text, units);
            UnmapViewOfFile(gpuzMemory);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
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
    int index = setup_value_type_index_->value();

    value = gpuzMemory->sensors[index].value;
    text = "";
    units = QString::fromWCharArray(gpuzMemory->sensors[index].unit);
    if(units=="%%") units = "%";

    return ;
}

void LH_GPUZData::loadTypesList(GPUZ_SH_MEM* gpuzMemory)
{
    setup_value_type_->list().clear();
    int i=0;
    while (i<256)
    {
        QString sensorName = QString::fromWCharArray( gpuzMemory->sensors[i++].name );
        if( sensorName == "" )
            break;
        else
            setup_value_type_->list().append( sensorName );
    }
    setup_value_type_->refreshList();
    setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_GPUZData::setTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_->setValue(setup_value_type_index_->value());
}

void LH_GPUZData::changeTypeSelection()
{
    if (setup_value_type_->list().length()!=0)
        setup_value_type_index_->setValue(setup_value_type_->value());
}
