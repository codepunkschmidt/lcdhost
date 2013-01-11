#include "LH_MonitoringSource_Afterburner.h"
#include "LH_MonitoringTypes_Afterburner.h"
#include <QRegExp>
#include <QSharedMemory>

LH_MonitoringSource_Afterburner::LH_MonitoringSource_Afterburner(LH_QtObject *parent): LH_MonitoringSource(parent, "MSI Afterburner") {}


bool LH_MonitoringSource_Afterburner::doUpdate()
{
#ifndef Q_OS_WIN32
    return false;
#else
    bool resultVal = false;

    HANDLE filemap = OpenFileMappingA(FILE_MAP_READ, FALSE, "MAHMSharedMemory");
    setDataAvailable(filemap != NULL);
    if(dataAvailable())
    {
        MAHM_SHARED_MEMORY_HEADER* MAHMHeader = (MAHM_SHARED_MEMORY_HEADER*)MapViewOfFile(filemap, FILE_MAP_READ, 0, 0, 0);

        if (MAHMHeader)
        {
            if (MAHMHeader->dwSignature == 0xDEAD)
                qDebug() << "LH_MonitoringSource_RivaTuner: Shared memory has been terminated; try again later.";
            else
            {
                QRegExp rx = QRegExp("([^0-9]*)([0-9]+)(.*)");
                rx.setPatternSyntax(QRegExp::RegExp2);
                for (uint i=0; i<MAHMHeader->dwNumEntries; i++)
                {
                    MAHM_SHARED_MEMORY_ENTRY* MAHMMemory = (MAHM_SHARED_MEMORY_ENTRY*)((uchar*)MAHMHeader + MAHMHeader->dwHeaderSize + i * MAHMHeader->dwEntrySize);
                    QString sensorName = QString(MAHMMemory->szSrcName);
                    QString sensorGroup = (rx.indexIn(reverse(sensorName))>-1? reverse(rx.cap(3)) + reverse(rx.cap(1)) : sensorName);

                    SensorDefinition def(
                                QString::fromLatin1(MAHMMemory->szSrcUnits),
                                minmax(
                                    OptionalValue(true, MAHMMemory->minLimit),
                                    OptionalValue(true, MAHMMemory->maxLimit)),
                                NA_
                                );
                    updateValue(sensorGroup,"",sensorName,(float)MAHMMemory->data, def);
                }
                foreach(SensorType type, sensors_)
                    foreach(SensorGroup group, type.groups)
                        updateAggregates(type.name, group.name);
                resultVal = true;
            }
            UnmapViewOfFile(MAHMHeader);
        }
        CloseHandle(filemap);
    }

    return resultVal;
#endif
}
