#ifndef LOGITECHDEVICEMANAGER_H
#define LOGITECHDEVICEMANAGER_H

#include "LogitechManager.h"

#ifdef Q_WS_WIN
# ifndef UNICODE
#  error ("This isn't going to work")
# endif
# include "windows.h"
# include "../wow64.h"
# include "win/lglcd.h"
#endif

#ifdef Q_WS_MAC
# include "mac/lgLcdError.h"
# include "mac/lgLcd.h"
# ifndef ERROR_FILE_NOT_FOUND
#  define ERROR_FILE_NOT_FOUND 2
# endif
# ifndef ERROR_PIPE_NOT_CONNECTED
#  define ERROR_PIPE_NOT_CONNECTED 2
# endif
# ifndef LGLCD_DEVICE_FAMILY_ALL
#  define LGLCD_DEVICE_FAMILY_ALL 0xFF
# endif
#endif

class LogitechOutputDevice;

class LogitechDeviceManager : public LogitechManager
{
    Q_OBJECT
    bool online_;
    LogitechOutputDevice *bw_;
    LogitechOutputDevice *qvga_;
    lgLcdConnectContextEx connectContextEx_;

public:
    explicit LogitechDeviceManager( QObject *parent = 0);
    ~LogitechDeviceManager();
    int connection() const { return connectContextEx_.connection; }
    void customEvent(QEvent *);

#ifdef Q_WS_WIN
    static DWORD WINAPI LH_LogitechCB( int connection, const PVOID pContext, DWORD code, DWORD p1, DWORD p2, DWORD p3, DWORD p4 );
#endif
#ifdef Q_WS_MAC
    static unsigned long LH_LogitechCB( int connection, const void* pContext, unsigned long code, unsigned long p1, unsigned long p2, unsigned long p3, unsigned long p4 );
#endif
};

#endif // LOGITECHDEVICEMANAGER_H
