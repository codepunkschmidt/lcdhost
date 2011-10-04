#include "LH_RivaTunerData.h"
#include <QDebug>
#include <QRegExp>
#include <algorithm>

LH_RivaTunerData::LH_RivaTunerData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups)
{
    Q_UNUSED(dataMode);

    ui_->reset(ui_mode_index);
    ui_->setVisible(mon_type | mon_item);
    ui_->showFormat();
    ui_->showOffset();
    ui_->refresh(mon_type | mon_item);
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

bool LH_RivaTunerData::getNames(QStringList& names)
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

int LH_RivaTunerData::lookupIndex(int lineID)
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

bool LH_RivaTunerData::getData(float& value, QString& text, QString& units)
{
    return (getData(value, text, units, ui_->getIndex()));
}

bool LH_RivaTunerData::getData(float& value, QString& text, QString& units, int index)
{
#ifdef Q_WS_WIN
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
                //qDebug() << "LH_RivaTuner: Shared memory has been terminated; try again later.";
                resultVal = false;
            }
            else
            {
                if (ui_->count(mon_type) == 0) loadTypesList(RTHMHeader);
                getSelectedValue(RTHMHeader, value, text, units, index);
            }
            UnmapViewOfFile(RTHMHeader);
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
    Q_UNUSED(index);
    Q_UNUSED(units);
    return false;
#endif
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
        for(int i=0; i<ui_->sensors_.count(); i++)
        {
            if(ui_->sensors_[i].indexes.contains(index))
            {
                value = 0;
                int valCount = 0;
                for(int j=0;j<ui_->sensors_[i].indexes.count(); j++)
                {
                    int index = ui_->sensors_[i].indexes[j];
                    if (index<(int)RTHMHeader->dwNumEntries && index >= 0)
                    {
                        valCount++;
                        RTHM_SHARED_MEMORY_ENTRY* RTHMMemory = (RTHM_SHARED_MEMORY_ENTRY*)((uchar*)(RTHMHeader + 1) + index * RTHMHeader->dwEntrySize);

                        float dataVal;
                        if (ui_->applyFormat())
                            dataVal = RTHMMemory->dataTransformed;
                        else
                            dataVal = RTHMMemory->data;
                        if (ui_->applyOffset()) dataVal += RTHMMemory->offset;

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
        RTHM_SHARED_MEMORY_ENTRY* RTHMMemory = (RTHM_SHARED_MEMORY_ENTRY*)((uchar*)(RTHMHeader + 1) + index * RTHMHeader->dwEntrySize);

        if (ui_->applyFormat())
            value = RTHMMemory->dataTransformed;
        else
            value = RTHMMemory->data;
        if (ui_->applyOffset()) value += RTHMMemory->offset;
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
        ui_->sensors_.clear();
        while (i<RTHMHeader->dwNumEntries)
        {
            RTHM_SHARED_MEMORY_ENTRY* RTHMMemory = (RTHM_SHARED_MEMORY_ENTRY*)((uchar*)(RTHMHeader + 1) + i * RTHMHeader->dwEntrySize);
            QString sensorName = QString(RTHMMemory->czSrc);
            if(rx.indexIn(reverse(sensorName)) >-1)
            {
                QString sensorGroupName = reverse(rx.cap(3)) + reverse(rx.cap(1));
                if(sensorGroupName.startsWith(":")) sensorGroupName.remove(0,1);
                sensorGroupName = sensorGroupName.trimmed();
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
        loadItemsList(ui_->value(mon_type));
    }
}

void LH_RivaTunerData::loadItemsList(int i)
{
    ui_->clear(mon_item);
    if(i < ui_->sensors_.length() && i >= 0)
        for(int j=0; j<ui_->sensors_[i].names.count(); j++)
            ui_->append(mon_item, ui_->sensors_[i].names[j]);
    ui_->refresh(mon_item);

    ui_->setVisible(mon_item, (ui_->count(mon_item)<=1));
}
