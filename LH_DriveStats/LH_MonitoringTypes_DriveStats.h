#ifndef LH_MONITORINGTYPES_DRIVESTATS_H
#define LH_MONITORINGTYPES_DRIVESTATS_H

#include <windows.h>
#include <winioctl.h>
#include <QString>
#include <QStringList>
#include <QMath.h>
#include <QHash>
#include <QDateTime>

struct DISK_PERFORMANCE_STATS {
    LARGE_INTEGER BytesRead;
    LARGE_INTEGER BytesWritten;
    LARGE_INTEGER ReadTime;
    LARGE_INTEGER WriteTime;
    LARGE_INTEGER IdleTime;
    DWORD         ReadCount;
    DWORD         WriteCount;
    DWORD         QueueDepth;
    DWORD         SplitCount;
    LARGE_INTEGER QueryTime;
    DWORD         StorageDeviceNumber;
    WCHAR         StorageManagerName[8];
  };

class DriveInfo
{
    DISK_PERFORMANCE_STATS diskPerformance;
    DISK_PERFORMANCE_STATS diskPerformanceOld;
    bool valid_;
    bool validOld_;
    bool validNew_;

    QDateTime lastUpdate_;

    ULARGE_INTEGER TotalBytes_;
    ULARGE_INTEGER FreeBytes_;

    QString drive_;

    bool GetDrivePerformanceInfo()
    {
        HANDLE hDevice = CreateFileA(QString("\\\\.\\%1:").arg(drive_).toAscii().data(),0,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);

        if(hDevice == INVALID_HANDLE_VALUE)
            return false;

        DWORD NumBytesReturned;
        bool bResult = DeviceIoControl(hDevice, IOCTL_DISK_PERFORMANCE, NULL, 0, &diskPerformance, sizeof(diskPerformance), &NumBytesReturned, NULL);

        CloseHandle(hDevice);
        return bResult;
    }

    QString getLastErrorMessage()
    {
        DWORD dwError;
        wchar_t errBuf[256];

        dwError = GetLastError();
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)errBuf, sizeof(errBuf),NULL);

        return QString("Err Code: %1 - %2").arg(dwError).arg(QString::fromWCharArray(errBuf));
    }

    bool setDrive(QString drive)
    {
        if(drive!="")
            drive = drive.at(0);
        if (drive_ != drive)
        {
            drive_ = drive;
            validOld_ = false;
            validNew_ = false;
            update();
            return true;
        }
        else
            return false;
    }


public:
    DriveInfo(QString drive = "")
    {
        setDrive(drive);
    }

    /*QStringList getDrives()
    {
        DWORD len = GetLogicalDriveStrings( 0, NULL );
        TCHAR buf[ len ];
        int buf_size = sizeof(buf) / sizeof(TCHAR);
        GetLogicalDriveStrings( buf_size, buf );

        QStringList ret_list;
        QString tmp = "";
        for( int ix = 0; ix < buf_size-1; ix++ )
        {
            if( buf[ix] == '\0' )
            {
                ret_list << tmp;
                tmp = "";
            }
            else
                tmp += (char)buf[ix];
        }

        return ret_list;
    }*/

    QString driveLetter() { return drive_; }

    bool valid()
    {
        return validOld_ && validNew_;
    }

    QDateTime lastUpdate()
    {
        return lastUpdate_;
    }

    bool update()
    {
        lastUpdate_ = QDateTime::currentDateTime();
        diskPerformanceOld = diskPerformance;
        validOld_ = validNew_;
        validNew_ = (drive_==""? false : GetDrivePerformanceInfo());
        if(validNew_)
        {
            ULARGE_INTEGER BytesAvailable;
            BytesAvailable.QuadPart = 0L;
            TotalBytes_.QuadPart = 0L;
            FreeBytes_.QuadPart = 0L;

            GetDiskFreeSpaceExA(QString("%1:\\").arg(drive_).toAscii().data(), &BytesAvailable, &TotalBytes_, &FreeBytes_);
        }
        return valid();
    }

    qlonglong BytesRead()
    {
        return(!valid()? 0 : diskPerformance.BytesRead.QuadPart - diskPerformanceOld.BytesRead.QuadPart);
    }

    qlonglong BytesWritten()
    {
        return(!valid()? 0 : diskPerformance.BytesWritten.QuadPart - diskPerformanceOld.BytesWritten.QuadPart);
    }

    qlonglong ReadCount()
    {
        return(!valid()? 0 : diskPerformance.ReadCount - diskPerformanceOld.ReadCount);
    }

    qlonglong WriteCount()
    {
        return(!valid()? 0 : diskPerformance.WriteCount - diskPerformanceOld.WriteCount);
    }

    double ReadTime()
    {
        qlonglong totaltime =
                (diskPerformance.ReadTime.QuadPart - diskPerformanceOld.ReadTime.QuadPart) +
                (diskPerformance.WriteTime.QuadPart - diskPerformanceOld.WriteTime.QuadPart) +
                (diskPerformance.IdleTime.QuadPart - diskPerformanceOld.IdleTime.QuadPart);
        return(!valid()? 0 : ((double)(diskPerformance.ReadTime.QuadPart - diskPerformanceOld.ReadTime.QuadPart))/totaltime*100);
    }

    double WriteTime()
    {
        qlonglong totaltime =
                (diskPerformance.ReadTime.QuadPart - diskPerformanceOld.ReadTime.QuadPart) +
                (diskPerformance.WriteTime.QuadPart - diskPerformanceOld.WriteTime.QuadPart) +
                (diskPerformance.IdleTime.QuadPart - diskPerformanceOld.IdleTime.QuadPart);
        return(!valid()? 0 : ((double)(diskPerformance.WriteTime.QuadPart - diskPerformanceOld.WriteTime.QuadPart))/totaltime*100);
    }

    double IdleTime()
    {
        qlonglong totaltime =
                (diskPerformance.ReadTime.QuadPart - diskPerformanceOld.ReadTime.QuadPart) +
                (diskPerformance.WriteTime.QuadPart - diskPerformanceOld.WriteTime.QuadPart) +
                (diskPerformance.IdleTime.QuadPart - diskPerformanceOld.IdleTime.QuadPart);
        return(!valid()? 0 : ((double)(diskPerformance.IdleTime.QuadPart - diskPerformanceOld.IdleTime.QuadPart))/totaltime*100);
    }

    qlonglong QueueDepth()
    {
        return(!validNew_? 0 : diskPerformance.QueueDepth);
    }

    qlonglong FreeSpace()
    {
        return(!validNew_? 0 : FreeBytes_.QuadPart);
    }

    qlonglong TotalSpace()
    {
        return(!validNew_? 0 : TotalBytes_.QuadPart);
    }

    qlonglong UsedSpace()
    {
        return(!validNew_? 0 : TotalBytes_.QuadPart - FreeBytes_.QuadPart);
    }
};

class DrivesList: public QHash<QString,DriveInfo>
{
    QStringList getDrives()
    {
        DWORD len = GetLogicalDriveStrings( 0, NULL );
        TCHAR buf[ len ];
        int buf_size = sizeof(buf) / sizeof(TCHAR);
        GetLogicalDriveStrings( buf_size, buf );

        QStringList ret_list;
        /*QString tmp = "";
        for( int ix = 0; ix < buf_size-1; ix++ )
        {
            if( buf[ix] == '\0' )
            {
                ret_list << tmp;
                tmp = "";
            }
            else
                tmp += (char)buf[ix];
        }*/
        ret_list.append("C");
        return ret_list;
    }

public:
    DrivesList()
    {
        QStringList drives = getDrives();
        foreach(QString drive, drives)
        {
            this->insert(drive, DriveInfo(drive));
        }
    }

    void update()
    {
        foreach(QString drive, this->keys())
        {
            (*this)[drive].update();
        }
    }

};

#endif // LH_MONITORINGTYPES_DRIVESTATS_H
