#include <QDebug>
#include <QRegExp>
#include <QSharedMemory>
#include <algorithm>

#include "LH_AfterburnerData.h"

LH_AfterburnerData::LH_AfterburnerData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups)
{
    Q_UNUSED(dataMode);

    ui_->reset(ui_mode_index);
    ui_->setVisible(mon_type | mon_item);
    ui_->showFormat();
    ui_->refresh(mon_type | mon_item);
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
    int index = ui_->getIndex();
    count = 0;
    if (index>=0)
        count = 1;
    else
        for(int i=0; i<ui_->sensors_.count(); i++)
            if(ui_->sensors_[i].indexes.contains(index))
                for(int j=0;j<ui_->sensors_[i].indexes.count(); j++)
                    if (ui_->sensors_[i].indexes[j]>=0) count ++;
    return true;
}

bool LH_AfterburnerData::getNames(QStringList& names)
{
    int index = ui_->getIndex();
    names.clear();
    if (index>=0)
        names.append("Default");
    else
        for(int i=0; i<ui_->sensors_.count(); i++)
            if(ui_->sensors_[i].indexes.contains(index))
                for(int j=0;j<ui_->sensors_[i].indexes.count(); j++)
                    if (ui_->sensors_[i].indexes[j]>=0)
                        names.append(ui_->sensors_[i].names[j]);
    return true;
}

int LH_AfterburnerData::lookupIndex(int lineID)
{
    int index = ui_->getIndex();
    if (index>=0)
        return index;
    else
        for(int i=0; i<ui_->sensors_.count(); i++)
            if(ui_->sensors_[i].indexes.contains(index))
                return ui_->sensors_[i].indexes[lineID];
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
    if (getData(value, resultText, units, format, min, max, ui_->getIndex()))
    {
        if (resultText=="") resultText = QString("%1").arg(value);
        if(ui_->applyFormat())
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
    return getData(value, text, units, format, min, max, ui_->getIndex());
}

bool LH_AfterburnerData::getGraphMin(float& min)
{
    float value;
    float max;
    QString format;
    QString units;
    QString text;
    return getData(value, text, units, format, min, max, ui_->getIndex());
}

bool LH_AfterburnerData::getData(float& value, QString& text, QString& units)
{
    float max;
    float min;
    QString format;
    return (getData(value, text, units, format, max, min, ui_->getIndex()));
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
#ifdef Q_WS_WIN
    bool resultVal = false;

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, "MAHMSharedMemory");
    if(filemap != NULL)
    {
        MAHM_SHARED_MEMORY_HEADER* MAHMHeader = (MAHM_SHARED_MEMORY_HEADER*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);

        if (MAHMHeader)
        {
            if (MAHMHeader->dwSignature == 0xDEAD)
            {
                qDebug() << "LH_Afterburner: Shared memory has been terminated; try again later.";
            }
            else
            {
                if (ui_->count(mon_type) == 0) loadTypesList(MAHMHeader);
                getSelectedValue(MAHMHeader, value, text, units, format, min, max, index);
                resultVal = true;
            }
            UnmapViewOfFile(MAHMHeader);
        }
        CloseHandle(filemap);
    }
    ui_->setReadonly(mon_type | mon_item, !resultVal);
    return resultVal;
#else
    Q_UNUSED(value);
    Q_UNUSED(text);
    Q_UNUSED(units);
    Q_UNUSED(format);
    Q_UNUSED(min);
    Q_UNUSED(max);
    Q_UNUSED(index);
    return false;
#endif
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
        for(int i=0; i<ui_->sensors_.count(); i++)
        {
            if(ui_->sensors_[i].indexes.contains(index))
            {
                value = 0;
                int valCount = 0;
                for(int j=0;j<ui_->sensors_[i].indexes.count(); j++)
                {
                    int index = ui_->sensors_[i].indexes[j];
                    if (index<(int)MAHMHeader->dwNumEntries && index >= 0)
                    {
                        valCount++;
                        MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((uchar*)MAHMHeader + MAHMHeader->dwHeaderSize + index * MAHMHeader->dwEntrySize);
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
        MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((uchar*)MAHMHeader + MAHMHeader->dwHeaderSize + index * MAHMHeader->dwEntrySize);

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
        ui_->sensors_.clear();
        while (i<MAHMHeader->dwNumEntries)
        {
            MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((uchar*)MAHMHeader + MAHMHeader->dwHeaderSize + i * MAHMHeader->dwEntrySize);
            QString sensorName = QString(MAHMMemory->szSrcName);
            if(rx.indexIn(reverse(sensorName)) >-1)
            {
                QString sensorGroupName = reverse(rx.cap(3)) + reverse(rx.cap(1));
                if(sensorNames.contains(sensorGroupName))
                {
                    int grpID = sensorNames.indexOf(sensorGroupName);
                    ui_->sensors_[grpID].names.append(sensorName);
                    ui_->sensors_[grpID].indexes.append(i);
                } else {
                    sensorNames.append( sensorGroupName );
                    sensorGroup grp;
                    grp.names.append(sensorName);
                    grp.indexes.append(i);
                    ui_->sensors_.append(grp);
                }
            } else {
                sensorNames.append( sensorName );
                sensorGroup grp;
                grp.names.append(sensorName);
                grp.indexes.append(i);
                ui_->sensors_.append(grp);
            }
            i++;
        }

        for(int i=0; i<ui_->sensors_.count(); i++)
            if(ui_->sensors_[i].names.count()>1)
            {
                int id = (ui_->sensors_[i].indexes[0]+1)*-10;
                ui_->sensors_[i].names << "Average" << "Lowest" << "Highest";
                ui_->sensors_[i].indexes << id-1 << id-2 << id-3;
                if(includeGroups_){
                    ui_->sensors_[i].names << "All";
                    ui_->sensors_[i].indexes << id-4;
                }
            }


        ui_->clear(mon_type);
        foreach(QString s,sensorNames)
            ui_->append( mon_type, s );

        ui_->refresh(mon_type);
        ui_->setIndex();
        ui_->loadItemsList(ui_->value(mon_type));
    }
}

