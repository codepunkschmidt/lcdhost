#ifndef LOGITECHLEGACYDEVICE_H
#define LOGITECHLEGACYDEVICE_H

#include "LogitechDevice.h"
#include "LogitechLegacyManager.h"

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
#endif

class LogitechLegacyDevice : public LogitechDevice
{
    Q_OBJECT
    bool toremove_;
    lgLcdOpenContext cxt_;

public:
    explicit LogitechLegacyDevice(bool bw, int index, LogitechLegacyManager *parent );

    int index() const { return cxt_.index; }
    int device() const { return cxt_.device; }

    bool toRemove() const { return toremove_; }
    void setToRemove(bool b) { toremove_ = b; }

    const char* open();
    const char* close();
};

#endif // LOGITECHLEGACYDEVICE_H
