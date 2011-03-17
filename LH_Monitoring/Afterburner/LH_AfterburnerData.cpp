#include "LH_AfterburnerData.h"
#include <QDebug>
#include <QRegExp>
#include <algorithm>

LH_AfterburnerData::LH_AfterburnerData(LH_QtObject *parent, bool includeGroups) : LH_MonitoringData( parent, includeGroups)
{
    setup_value_type_ = new LH_Qt_QStringList(parent,"Value Type", QStringList(),LH_FLAG_READONLY);
    setup_value_type_->setHelp( "<p>The type of value to display.</p>");
    setup_value_type_->setOrder(-4);

    setup_value_item_ = new LH_Qt_QStringList(parent,"Value Item", QStringList(),LH_FLAG_READONLY);
    setup_value_item_->setHelp( "<p>The value to display.</p>");
    setup_value_item_->setOrder(-4);

    setup_value_index_ = new LH_Qt_int(parent,"(Value Index)", 0, -1000, 1000, LH_FLAG_HIDDEN);
    setup_value_index_->setOrder(-4);


    setup_value_format_ = new LH_Qt_bool(parent,"Apply Formatting", false, LH_FLAG_HIDDEN);
    setup_value_format_->setOrder(-4);

    parent->connect(setup_value_type_, SIGNAL(changed()), parent, SLOT(changeTypeSelection()) );
    parent->connect(setup_value_item_, SIGNAL(changed()), parent, SLOT(changeItemSelection()) );
    parent->connect(setup_value_index_, SIGNAL(changed()), parent, SLOT(setItemSelection()) );
}

LH_AfterburnerData::~LH_AfterburnerData()
{

}

monitoringDataType LH_AfterburnerData::getType()
{
    if(is_avg_)
        return mdtFloat;
    else
        return mdtInt;
}

bool LH_AfterburnerData::getCount(int& count)
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

bool LH_AfterburnerData::getNames(QStringList& names)
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

int LH_AfterburnerData::lookupIndex(int lineID)
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

QString LH_AfterburnerData::getText(bool addUnits)
{
    QString units;
    float value;
    float min;
    float max;

    QString resultText;
    QString format;
    if (getData(value, resultText, units, format, min, max, getIndex()))
    {
        if (resultText=="") resultText = QString("%1").arg(value);
        if(setup_value_format_->value())
        {
            char* formattedValue = (char*)"";
            sprintf(formattedValue, (const char*)format.toLatin1(), (const char*)resultText.toLatin1());
            resultText = formattedValue;
        }
        if(addUnits) resultText = resultText + units;
    } else
        resultText = "N/A";

    return resultText;
}

bool LH_AfterburnerData::getGraphMax(float& max)
{
    float value;
    float min;
    QString format;
    QString units;
    QString text;
    return getData(value, text, units, format, min, max, getIndex());
}

bool LH_AfterburnerData::getGraphMin(float& min)
{
    float value;
    float max;
    QString format;
    QString units;
    QString text;
    return getData(value, text, units, format, min, max, getIndex());
}

bool LH_AfterburnerData::getData(float& value, QString& text, QString& units)
{
    float max;
    float min;
    QString format;
    return (getData(value, text, units, format, max, min, getIndex()));
}
bool LH_AfterburnerData::getData(float& value, QString& text, QString& units, int index)
{
    float max;
    float min;
    QString format;
    return (getData(value, text, units, format, max, min, index));
}

bool LH_AfterburnerData::getData(float& value, QString& text, QString& units, QString& format, float& min, float& max, int index)
{
    bool resultVal = true;
#ifdef Q_WS_WIN
    const char* mapname = "MAHMSharedMemory";

    // Create file mapping
    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, mapname);
    // Get pointer
    if(filemap != NULL)
    {
        MAHM_SHARED_MEMORY_HEADER* MAHMHeader = (MAHM_SHARED_MEMORY_HEADER*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);

        if (MAHMHeader) {


            if (MAHMHeader->dwSignature == 0xDEAD)
            {
                qDebug() << "LH_Afterburner: Shared memory has been terminated; try again later.";
                resultVal = false;
            }
            else
            {
                if (setup_value_type_->list().length() == 0) loadTypesList(MAHMHeader);
                getSelectedValue(MAHMHeader, value, text, units, format, min, max, index);
            }
            UnmapViewOfFile(MAHMHeader);
        } else
            resultVal = false;
        CloseHandle(filemap);
    } else
        resultVal = false;

    setup_value_type_->setFlag(LH_FLAG_READONLY, !resultVal);
    setup_value_item_->setFlag(LH_FLAG_READONLY, !resultVal);
#else
    resultVal = false;
#endif
    return resultVal;
}

void LH_AfterburnerData::getSelectedValue(MAHM_SHARED_MEMORY_HEADER* MAHMHeader, float& value, QString& text, QString& units, QString& format, float& min, float& max, int index)
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
                    if (index<(int)MAHMHeader->dwNumEntries && index >= 0)
                    {
                        valCount++;
                        MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((LPBYTE)MAHMHeader + MAHMHeader->dwHeaderSize + index * MAHMHeader->dwEntrySize);
                        //qDebug() << "Value[" << j <<"] = " << MAHMMemory->data;
                        if (j==0)
                            value = MAHMMemory->data;
                        else switch(aggType)
                        {
                        case -1:
                            value += MAHMMemory->data;
                            break;
                        case -2:
                            value = qMin(value,MAHMMemory->data);
                            break;
                        case -3:
                            value = qMax(value,MAHMMemory->data);
                            break;
                        default:
                            value = 0;
                        }

                        units = QString(MAHMMemory->szSrcUnits);
                        format = QString(MAHMMemory->szRecommendedFormat);
                        min = MAHMMemory->minLimit;
                        max = MAHMMemory->maxLimit;
                    }
                }
                //qDebug() << "Result: " << value << " (from " << valCount << ")";
                if(aggType==-1) value /= valCount;
                //qDebug() << "Final: " << value;
            }
        }
    } else
    if (index<(int)MAHMHeader->dwNumEntries)
    {
        MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((LPBYTE)MAHMHeader + MAHMHeader->dwHeaderSize + index * MAHMHeader->dwEntrySize);

        value = MAHMMemory->data;
        text = "";
        units = QString(MAHMMemory->szSrcUnits);
        format = QString(MAHMMemory->szRecommendedFormat);
        min = MAHMMemory->minLimit;
        max = MAHMMemory->maxLimit;
    } else {
        value = 0;
        units = "";
        text = "";
        format = "%0";
        min = 0;
        max = 1;
    }

    return ;
}

QString LH_AfterburnerData::reverse(QString str)
{
    QByteArray ba = str.toAscii();
    char *d = ba.data();
    std::reverse(d, d+str.length());
    return QString(d);
}

void LH_AfterburnerData::loadTypesList(MAHM_SHARED_MEMORY_HEADER* MAHMHeader)
{
    if(MAHMHeader)
    {
        uint i=0;
        QStringList sensorNames;
        QRegExp rx = QRegExp("([^0-9]*)([0-9]+)(.*)");
        rx.setPatternSyntax(QRegExp::RegExp2);
        sensors_.clear();
        while (i<MAHMHeader->dwNumEntries)
        {
            MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((LPBYTE)MAHMHeader + MAHMHeader->dwHeaderSize + i * MAHMHeader->dwEntrySize);
            QString sensorName = QString(MAHMMemory->szSrcName);
            if(rx.indexIn(reverse(sensorName)) >-1)
            {
                QString sensorGroupName = reverse(rx.cap(3)) + reverse(rx.cap(1));
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

void LH_AfterburnerData::setTypeSelection()
{
    setIndex();
}

void LH_AfterburnerData::changeTypeSelection()
{
    loadItemsList( setup_value_type_->value() );
    setup_value_index_->setValue( getIndex() );
}

void LH_AfterburnerData::setItemSelection()
{
    setIndex();
}

void LH_AfterburnerData::changeItemSelection()
{
    setup_value_index_->setValue(getIndex());

    float value;
    QString units;
    QString text;
    getData(value, text, units);
}

int LH_AfterburnerData::getIndex()
{
    int typeID = setup_value_type_->value();
    int itemID = setup_value_item_->value();
    if(typeID < sensors_.length() && typeID != -1)
        if(itemID < sensors_[typeID].indexes.length() && itemID!=-1)
            return sensors_[typeID].indexes[itemID];
    return 0;
}

void LH_AfterburnerData::setIndex(int index)
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

void LH_AfterburnerData::loadItemsList(int i)
{
    setup_value_item_->list().clear();
    if(i < sensors_.length() && i >= 0)
        for(int j=0; j<sensors_[i].names.count(); j++)
            setup_value_item_->list().append(sensors_[i].names[j]);
    setup_value_item_->refreshList();

    setup_value_item_->setFlag(LH_FLAG_HIDDEN , (setup_value_item_->list().count()<=1));
}
