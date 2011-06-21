#include <QFile>

#include "LH_QtPlugin_Mailcount.h"

#ifdef Q_WS_WIN
#include <windows.h>
/* Make sure unread mail function is declared */
typedef HRESULT (WINAPI *SHGetUnreadMailCountW_t)(
    HKEY hKeyUser,
    LPCTSTR pszMailAddress,
    DWORD *pdwCount,
    FILETIME *pFileTime,
    LPCTSTR pszShellExecuteCommand,
    int cchShellExecuteCommand
);
static HANDLE hShell32Dll = (HANDLE)0;
static SHGetUnreadMailCountW_t SHGetUnreadMailCountW = NULL;
#endif

LH_QtPlugin_Mailcount *LH_QtPlugin_Mailcount::instance_ = 0;

LH_PLUGIN(LH_QtPlugin_Mailcount)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Mailcount</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Unread mail indicator."
  "</shortdesc>"
  "<longdesc>"
  "Show unread e-mails. Requires an e-mail client with shell support, like Mozilla Thunderbird or Microsoft Outlook."
  "</longdesc>"
"</lcdhostplugin>";

LH_QtPlugin_Mailcount::LH_QtPlugin_Mailcount()
{
    Q_ASSERT( instance_ == 0 );
    email_count_ = new LH_Qt_int(this,tr("Current count"),0,LH_FLAG_READONLY);
    email_addr_ = new LH_Qt_QString(this,tr("Only check address"),QString(),LH_FLAG_AUTORENDER);
    email_days_ = new LH_Qt_int(this,tr("Days back to check"),7,LH_FLAG_AUTORENDER);
    check_interval_ = new LH_Qt_int(this,tr("Check interval (seconds)"),2,LH_FLAG_AUTORENDER);
    instance_ = this;
}

LH_QtPlugin_Mailcount::~LH_QtPlugin_Mailcount()
{
    Q_ASSERT( instance_ != 0 );
    instance_ = 0;
}

const char *LH_QtPlugin_Mailcount::init( lh_callback_t cb, int cb_id, const char *name, const lh_systemstate* state )
{
#ifdef Q_WS_WIN
    hShell32Dll = LoadLibraryW( L"SHELL32.DLL" );
    if( hShell32Dll != NULL )
        SHGetUnreadMailCountW = (SHGetUnreadMailCountW_t) GetProcAddress( (HMODULE) hShell32Dll, "SHGetUnreadMailCountW" );
#endif
    return NULL;
}

int LH_QtPlugin_Mailcount::notify( int code, void *param )
{
    if( !code || (code & LH_NOTE_SECOND) )
    {
        int delta = last_check_.secsTo( QTime::currentTime() );
        if( !last_check_.isValid() ||
            delta < 0 ||
            delta > check_interval_->value() )
        {
            last_check_ = QTime::currentTime();
            getUnreadMailcount();
        }
    }

    return LH_QtPlugin::notify( code, param ) | LH_NOTE_SECOND;
}

void LH_QtPlugin_Mailcount::term()
{
#ifdef Q_WS_WIN
    if( hShell32Dll )
    {
            FreeLibrary( (HMODULE) hShell32Dll );
            hShell32Dll = NULL;
            SHGetUnreadMailCountW = 0;
    }
#endif
    return;
}

int LH_QtPlugin_Mailcount::getUnreadMailcount()
{
    int total = 0;

#ifdef Q_WS_WIN
    if( SHGetUnreadMailCountW )
    {
        HRESULT retv;
        SYSTEMTIME st;
        FILETIME ft;
        ULARGE_INTEGER mailtime;
        DWORD dwMail = 0;

        // Get unread mail count
        GetLocalTime( &st );
        SystemTimeToFileTime( &st, &ft );
        Q_ASSERT( sizeof(mailtime) == sizeof(ft) );
        memcpy( &mailtime, &ft, sizeof(mailtime) );
        mailtime.QuadPart -= (ULONGLONG)email_days_->value()*24*60*60*10000000; // subtract wanted number of days
        memcpy( &ft, &mailtime, sizeof(mailtime) );
        if( !email_addr_->value().isEmpty() )
        {
            retv = SHGetUnreadMailCountW( NULL, (LPCTSTR)(void*)email_addr_->value().utf16(), &dwMail, &ft, NULL, 0 );
        }
        else
            retv = SHGetUnreadMailCountW( NULL, NULL, &dwMail, &ft, NULL, 0 );
        if( retv != S_OK ) dwMail = 0;

        total += dwMail;
    }
#endif

    email_count_->setValue(total);
    return total;
}
