#include "LH_DriveStatsData.h"

#include <QDebug>
#include <QRegExp>
#include <algorithm>
#include <QSettings>

LH_DriveStatsData::LH_DriveStatsData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups) : LH_MonitoringData( parent, ui, dataMode, includeGroups )
{
    Q_UNUSED(includeGroups);

    ui_->reset();
    ui_->setVisible(mon_item | mon_type);

    if(dataMode & mdmNumbers) ui_->append(mon_item, "Bytes Read");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Bytes Written");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Read Count");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Write Count");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Read Time");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Write Time");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Idle Time");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Queue Depth");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Free Space");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Used Space");
    if(dataMode & mdmNumbers) ui_->append(mon_item, "Total Space");

    if(dataMode & mdmPie) ui_->append(mon_item, "Disk Space");

    QStringList drives = driveInfo.getDrives();
    for(int i=0; i<drives.count(); i++)
        ui_->append(mon_type, drives.at(i) );

    driveInfo.setDrive(ui_->valueText(mon_type));

    ui_->refresh(mon_item | mon_type);
}

bool LH_DriveStatsData::getData(float& value, QString& text, QString& units)
{

    value = 0;
    text = "";
    units = "";

    if(driveInfo.setDrive(ui_->valueText(mon_type)))
    {
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Bytes Read");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Bytes Written");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Read Count");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Write Count");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Read Time");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Write Time");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Idle Time");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Queue Depth");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Free Space");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Used Space");
        if(dataMode_ & mdmNumbers) ui_->append(mon_item, "Total Space");

        if(dataMode_ & mdmPie) ui_->append(mon_item, "Disk Space");

        ui_->refresh(mon_item);
    }
    else
        driveInfo.update();
    if(driveInfo.valid())
    {
        if(ui_->valueText(mon_item) == "Bytes Read")
            parseBytes(driveInfo.BytesRead(), value, text, units);
        if(ui_->valueText(mon_item) == "Bytes Written")
            parseBytes(driveInfo.BytesWritten(), value, text, units);
        if(ui_->valueText(mon_item) == "Read Count")
            value = driveInfo.ReadCount();
        if(ui_->valueText(mon_item) == "Write Count")
            value = driveInfo.WriteCount();
        if(ui_->valueText(mon_item) == "Read Time")
        {
            value = driveInfo.ReadTime();
            units = "%";
        }
        if(ui_->valueText(mon_item) == "Write Time")
        {
            value = driveInfo.WriteTime();
            units = "%";
        }
        if(ui_->valueText(mon_item) == "Idle Time")
        {
            value = driveInfo.IdleTime();
            units = "%";
        }
        if(ui_->valueText(mon_item) == "Queue Depth")
            value = driveInfo.QueueDepth();
        if(ui_->valueText(mon_item) == "Free Space")
            parseBytes(driveInfo.FreeSpace(), value, text, units);
        if(ui_->valueText(mon_item) == "Used Space")
            parseBytes(driveInfo.UsedSpace(), value, text, units);
        if(ui_->valueText(mon_item) == "Total Space")
            parseBytes(driveInfo.TotalSpace(), value, text, units);

        if(ui_->valueText(mon_item) == "Disk Space")
            value = driveInfo.UsedSpace();
    }

    return driveInfo.valid();
}

bool LH_DriveStatsData::getPieMax(float &value)
{
    value = driveInfo.TotalSpace();
    return driveInfo.valid();
}


void LH_DriveStatsData::parseBytes(qlonglong bytes, float& value, QString& text, QString& units)
{
    value = bytes;
    units = "B";
    while(value >= 1024 && units != "TB")
    {
        value /= 1024;
        if(units=="B") units = "kB"; else
            if(units=="kB") units = "MB"; else
                if(units=="MB") units = "GB"; else
                    if(units=="GB") units = "TB";
    }
    text = QString::number(value, 'f', 0);
    value = bytes;
}

monitoringDataType LH_DriveStatsData::getType()
{
    if(ui_->valueText(mon_item).endsWith("Time"))
        return mdtFloat;
    return mdtInt;
}

