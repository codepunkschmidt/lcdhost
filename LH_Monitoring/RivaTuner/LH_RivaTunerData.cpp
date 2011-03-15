#include "LH_RivaTunerData.h"
#include <QDebug>
#include <QRegExp>
#include <algorithm>

LH_RivaTunerData::LH_RivaTunerData(LH_QtObject *parent, bool includeGroups) : LH_MonitoringData( parent, includeGroups)
{
    setup_value_type_ = new LH_Qt_QStringList(parent,"Value Type", QStringList(),LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_item_ = new LH_Qt_QStringList(parent,"Value Item", QStringList(),LH_FLAG_READONLY);
    setup_value_item_->setHelp( "<p>The value to display.</p>");
    setup_value_item_->setOrder(-4);

    setup_value_index_ = new LH_Qt_int(parent,"(Value Index)", 0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_index_->setOrder(-4);

    setup_value_offset_ = new LH_Qt_bool(parent,"Apply Offsets", true);
    setup_value_offset_->setHelp( "<p>Add any user defined offsets to the value (defined within RivaTuner).</p>");
    setup_value_offset_->setOrder(-4);

    setup_value_format_ = new LH_Qt_bool(parent,"Use Transformed Data", false);
    setup_value_offset_->setHelp( "<p>Relates to RivaTuner's \"raw data transforming mode\".</p><p>(If you don't know what this is, leave it disabled and ignore it.)</p>");
    setup_value_format_->setHelp( "<p>The value to display.</p>");
    setup_value_format_->setOrder(-4);

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_item_, SIGNAL(changed()), parent, SLOT(changeItemSelection()) );
    parent->connect(setup_value_index_, SIGNAL(changed()), parent, SLOT(setItemSelection()) );
}

LH_RivaTunerData::~LH_RivaTunerData()
{

}

monitoringDataType LH_RivaTunerData::getType()
{
    if(is_avg_)
        return mdtFloat;
    else
        return mdtInt;
}

bool LH_RivaTunerData::getCount(int& count)
{
    int index = getIndex();
    count = 0;
    if (index>=0)
        count = 1;
    else
        for(int i=0; i<sensors_.count(); i++)
            if(sensors_[i].indexes.contains(index))
                for(int j=0;j<sensors_[i].indexes.count(); j++)
                    if (sensors_[i].indexes[j]>=0) count ++;
    return true;
}

bool LH_RivaTunerData::getNames(QStringList& names)
{
    int index = getIndex();
    names.clear();
    if (index>=0)
        names.append("Default");
    else
        for(int i=0; i<sensors_.count(); i++)
            if(sensors_[i].indexes.contains(index))
                for(int j=0;j<sensors_[i].indexes.count(); j++)
                    if (sensors_[i].indexes[j]>=0)
                        names.append(sensors_[i].names[j]);
    return true;
}

int LH_RivaTunerData::lookupIndex(int lineID)
{
    int index = getIndex();
    if (index>=0)
        return index;
    else
        for(int i=0; i<sensors_.count(); i++)
            if(sensors_[i].indexes.contains(index))
                return sensors_[i].indexes[lineID];
    return 0;
}

bool LH_RivaTunerData::getData(float& value, QString& text, QString& units)
{
    return (getData(value, text, units, getIndex()));
}
bool LH_RivaTunerData::getData(float& value, QString& text, QString& units, int index)
{
    const char* mapname = "RTHMSharedMemory";
    bool resultVal = true;

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    // Get pointer
    if(filemap != NULL)
    {
        RTHM_SHARED_MEMORY_HEADER* RTHMHeader = (RTHM_SHARED_MEMORY_HEADER*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);

        if (RTHMHeader) {
            if (RTHMHeader->dwSignature == 0xDEAD)
            {
                qDebug() << "LH_RivaTuner: Shared memory has been terminated; try again later.";
                resultVal = false;
            }
            else
            {
                if (setup_value_type_->list().length() == 0) loadTypesList(RTHMHeader);
                getSelectedValue(RTHMHeader, value, text, units, index);
            }
            UnmapViewOfFile(RTHMHeader);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    setup_value_item_->setFlag(LH_FLAG_READONLY, !resultVal);
    return resultVal;
}

void LH_RivaTunerData::getSelectedValue(RTHM_SHARED_MEMORY_HEADER* RTHMHeader, float& value, QString& text, QString& units, int index)
{
    is_avg_ = false;
    if (index<0)
    {
        int aggType = index % 10;
        is_avg_ = (aggType == -1);
        is_group_ = (aggType == -4);
        //int baseItem = ((index + aggType) / 10) + 1;
        //qDebug() << aggType << " [" << baseItem << "]";
        for(int i=0; i<sensors_.count(); i++)
        {
            if(sensors_[i].indexes.contains(index))
            {
                value = 0;
                int valCount = 0;
                for(int j=0;j<sensors_[i].indexes.count(); j++)
                {
                    int index = sensors_[i].indexes[j];
                    if (index<(int)RTHMHeader->dwNumEntries && index >= 0)
                    {
                        valCount++;
                        RTHM_SHARED_MEMORY_ENTRY* RTHMMemory = (RTHM_SHARED_MEMORY_ENTRY*)((LPBYTE)(RTHMHeader + 1) + index * RTHMHeader->dwEntrySize);

                        float dataVal;
                        if (setup_value_format_->value())
                            dataVal = RTHMMemory->dataTransformed;
                        else
                            dataVal = RTHMMemory->data;
                        if (setup_value_offset_->value()) dataVal += RTHMMemory->offset;

                        if (j==0)
                            value = dataVal;
                        else switch(aggType)
                        {
                        case -1:
                            value += dataVal;
                            break;
                        case -2:
                            value = qMin(value,dataVal);
                            break;
                        case -3:
                            value = qMax(value,dataVal);
                            break;
                        default:
                            value = 0;
                        }

                        units = QString(RTHMMemory->czDim);
                    }
                }
                //qDebug() << "Result: " << value << " (from " << valCount << ")";
                if(aggType==-1) value /= valCount;
                //qDebug() << "Final: " << value;
            }
        }
    } else
    if (index<(int)RTHMHeader->dwNumEntries)
    {
        RTHM_SHARED_MEMORY_ENTRY* RTHMMemory = (RTHM_SHARED_MEMORY_ENTRY*)((LPBYTE)(RTHMHeader + 1) + index * RTHMHeader->dwEntrySize);

        if (setup_value_format_->value())
            value = RTHMMemory->dataTransformed;
        else
            value = RTHMMemory->data;
        if (setup_value_offset_->value()) value += RTHMMemory->offset;
        text = "";
        units = QString(RTHMMemory->czDim);
    } else {
        value = 0;
        units = "";
        text = "";
    }

    return ;
}

QString LH_RivaTunerData::reverse(QString str)
{
    QByteArray ba = str.toAscii();
    char *d = ba.data();
    std::reverse(d, d+str.length());
    return QString(d);
}

void LH_RivaTunerData::loadTypesList(RTHM_SHARED_MEMORY_HEADER* RTHMHeader)
{
    if(RTHMHeader)
    {
        uint i=0;
        QStringList sensorNames;
        QRegExp rx = QRegExp("([^#0-9]*)([#0-9]+)(.*)");
        rx.setPatternSyntax(QRegExp::RegExp2);
        sensors_.clear();
        while (i<RTHMHeader->dwNumEntries)
        {
            RTHM_SHARED_MEMORY_ENTRY* RTHMMemory = (RTHM_SHARED_MEMORY_ENTRY*)((LPBYTE)(RTHMHeader + 1) + i * RTHMHeader->dwEntrySize);
            QString sensorName = QString(RTHMMemory->czSrc);
            if(rx.indexIn(reverse(sensorName)) >-1)
            {
                QString sensorGroupName = reverse(rx.cap(3)) + reverse(rx.cap(1));
                if(sensorGroupName.startsWith(":")) sensorGroupName.remove(0,1);
                sensorGroupName = sensorGroupName.trimmed();
                if(sensorNames.contains(sensorGroupName))
                {
                    int grpID = sensorNames.indexOf(sensorGroupName);
                    sensors_[grpID].names.append(sensorName);
                    sensors_[grpID].indexes.append(i);
                } else {
                    sensorNames.append( sensorGroupName );
                    sensorGroup grp;
                    grp.names.append(sensorName);
                    grp.indexes.append(i);
                    sensors_.append(grp);
                }
            } else {
                sensorNames.append( sensorName );
                sensorGroup grp;
                grp.names.append(sensorName);
                grp.indexes.append(i);
                sensors_.append(grp);
            }
            i++;
        }

        for(int i=0; i<sensors_.count(); i++)
            if(sensors_[i].names.count()>1)
            {
                int id = (sensors_[i].indexes[0]+1)*-10;
                sensors_[i].names << "Average" << "Lowest" << "Highest";
                sensors_[i].indexes << id-1 << id-2 << id-3;
                if(includeGroups_){
                    sensors_[i].names << "All";
                    sensors_[i].indexes << id-4;
                }
            }


        setup_value_type_->list().clear();
        foreach(QString s,sensorNames)
            setup_value_type_->list().append( s );

        setup_value_type_->refreshList();
        setIndex();
        loadItemsList(setup_value_type_->value());
    }
}

void LH_RivaTunerData::setTypeSelection()
{
    setIndex();
}

void LH_RivaTunerData::changeTypeSelection()
{
    loadItemsList( setup_value_type_->value() );
    setup_value_index_->setValue( getIndex() );
}

void LH_RivaTunerData::setItemSelection()
{
    if(sensors_.count() == 0){
        float value; QString text; QString units;
        getData(value, text, units, 0);
    }
    setIndex();
}

void LH_RivaTunerData::changeItemSelection()
{
    setup_value_index_->setValue(getIndex());

    float value;
    QString units;
    QString text;
    getData(value, text, units);
}

int LH_RivaTunerData::getIndex()
{
    int typeID = setup_value_type_->value();
    int itemID = setup_value_item_->value();
    if(typeID < sensors_.length() && typeID != -1)
        if(itemID < sensors_[typeID].indexes.length())
            return sensors_[typeID].indexes[itemID];
    return 0;
}

void LH_RivaTunerData::setIndex(int index)
{
    if(index==-1) index = setup_value_index_->value();
    for(int i=0; i<sensors_.count(); i++)
    {
        if(sensors_[i].indexes.contains(index))
        {
            if(setup_value_type_->value()!=i || setup_value_item_->list().count()==0)
            {
                setup_value_type_->setValue(i);
                loadItemsList(i);
            }
            setup_value_item_->setValue(sensors_[i].indexes.indexOf(index));
            return;
        }
    }
    setup_value_type_->setValue(-1);
    setup_value_item_->setValue(-1);
}

void LH_RivaTunerData::loadItemsList(int i)
{
    setup_value_item_->list().clear();
    if(i < sensors_.length() && i >= 0)
        for(int j=0; j<sensors_[i].names.count(); j++)
            setup_value_item_->list().append(sensors_[i].names[j]);
    setup_value_item_->refreshList();

    setup_value_item_->setFlag(LH_FLAG_HIDDEN , (setup_value_item_->list().count()<=1));
}
