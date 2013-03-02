#include "LH_MonitoringSource_DriveStats.h"
//#include "LH_MonitoringTypes_DriveStats.h"

LH_MonitoringSource_DriveStats::LH_MonitoringSource_DriveStats(LH_QtObject *parent): LH_MonitoringSource(parent, "Drive Stats"), drives_()
{
    defPerc_ = SensorDefinition("%");

    defBytes_ = SensorDefinition("B");
    defBytes_.setAdaptiveUnits(1024, QStringList()<<"B"<<"kB"<<"MB"<<"GB"<<"TB");

    // Adaptive Unit behaviour will need to be coded seperately for:
    // o LH_MonitoringObject
    // o LH_Graph (due to it's special data access method.)
}

bool LH_MonitoringSource_DriveStats::doUpdate()
{
#ifndef Q_OS_WIN
    return false;
#else
    bool resultVal = true;
    setDataAvailable(true);

    drives_.update();

    foreach(DriveInfo drive, drives_.values())
    {
        updateValue("Bytes Read/Written", "Bytes Read", drive.driveLetter(), drive.BytesRead(), defBytes_);
        updateValue("Bytes Read/Written", "Bytes Written", drive.driveLetter(), drive.BytesWritten(), defBytes_);

        updateValue("Activity Counters", "Read Count", drive.driveLetter(), drive.ReadCount());
        updateValue("Activity Counters", "Write Count", drive.driveLetter(), drive.WriteCount());

        updateValue("Activity Percentages", "Read Time", drive.driveLetter(), drive.ReadTime(), defPerc_);
        updateValue("Activity Percentages", "Write Time", drive.driveLetter(), drive.WriteTime(), defPerc_);
        updateValue("Activity Percentages", "Idle Time", drive.driveLetter(), drive.IdleTime(), defPerc_);

        updateValue("Queue Depth", "", drive.driveLetter(), drive.QueueDepth());

        updateValue("Disk Space", "Free Space", drive.driveLetter(), drive.FreeSpace(), defBytes_);
        updateValue("Disk Space", "Used Space", drive.driveLetter(), drive.UsedSpace(), defBytes_);
        updateValue("Disk Space", "Total Space", drive.driveLetter(), drive.TotalSpace(), defBytes_);

        updateValue("Last Update", "", drive.driveLetter(), drive.lastUpdate().toString());

        //Pie: "Disk Space"
    }

    return resultVal;
#endif
}
