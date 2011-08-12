#ifndef LOGITECHLEGACYMANAGER_H
#define LOGITECHLEGACYMANAGER_H

#include <QBasicTimer>
#include "LogitechManager.h"

class LogitechLegacyDevice;

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

class LogitechLegacyManager : public LogitechManager
{
    Q_OBJECT
    bool online_;
    lgLcdConnectContext connectContext_;
    QBasicTimer enum_timer_;

public:
    explicit LogitechLegacyManager(QObject *parent = 0);
    ~LogitechLegacyManager();
    int connection() const { return connectContext_.connection; }
    void enumerate();
    void timerEvent(QTimerEvent *);
};

#endif // LOGITECHLEGACYMANAGER_H
