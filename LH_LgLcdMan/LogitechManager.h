#ifndef LOGITECHMANAGER_H
#define LOGITECHMANAGER_H

#include <QObject>

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

#define LCD_ERR(x) ( LogitechManager::lglcd_Err((x),__FILE__,__LINE__) == NULL )

class LogitechManager : public QObject
{
    Q_OBJECT

protected:
#ifdef Q_WS_WIN
    wchar_t *appname_;
#endif
#ifdef Q_WS_MAC
    CFStringRef appname_;
#endif

public:
    explicit LogitechManager(QObject *parent = 0);
    ~LogitechManager();

    virtual int connection() const { return LGLCD_INVALID_CONNECTION; }

    static const char *lglcd_Err( int result, const char *filename = NULL, unsigned line = 0 );
};

#endif // LOGITECHMANAGER_H
