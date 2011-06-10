#include <QFile>

#include "LH_QtPlugin_Mailcount.h"

/**************************************************************************
** LH_QtPlugin_Mailcount
**************************************************************************/

LH_PLUGIN(LH_QtPlugin_Mailcount)
lh_buildinfo buildinfo = LH_STD_BUILDINFO;

const lh_blob *LH_QtPlugin_Mailcount::lh_logo()
{
    static char logo_blob_data[ sizeof(lh_blob) + 3405 ]; // 3405 = filesize of 'envelope48.png'
    static lh_blob *logo_blob = NULL;

    if( logo_blob == NULL )
    {
        logo_blob = (lh_blob *) (void*) & logo_blob_data;
        QFile file(":/mailcount/images/envelope48.png");
        file.open(QIODevice::ReadOnly);
        QByteArray array = file.readAll();
        Q_ASSERT( array.size() + sizeof(lh_blob) == sizeof(logo_blob_data) );
        memset( logo_blob_data, 0, sizeof(logo_blob_data) );
        logo_blob->len = array.size();
        logo_blob->sign = 0xDEADBEEF;
        memcpy( & logo_blob->data, array.constData(), array.size() );
    }

    return logo_blob;
}

const char *LH_QtPlugin_Mailcount::lh_load()
{
#ifdef Q_WS_WIN
    hShell32Dll = LoadLibraryW( L"SHELL32.DLL" );
    if( hShell32Dll != NULL )
        SHGetUnreadMailCountW = (SHGetUnreadMailCountW_t) GetProcAddress( (HMODULE) hShell32Dll, "SHGetUnreadMailCountW" );
#endif
    return NULL;
}

void LH_QtPlugin_Mailcount::lh_unload()
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

LH_QtPlugin_Mailcount::LH_QtPlugin_Mailcount()
{
    email_addr_ = 0;
    email_days_ = 0;
    check_interval_ = 0;
    email_addr_ = new LH_Qt_QString(this,tr("Only check address"),QString(),LH_FLAG_AUTORENDER);
    email_days_ = new LH_Qt_int(this,tr("Days back to check"),7,LH_FLAG_AUTORENDER);
    check_interval_ = new LH_Qt_int(this,tr("Check interval (seconds)"),2,LH_FLAG_AUTORENDER);
}

const char *LH_QtPlugin_Mailcount::init( const char *name, const lh_systemstate* state )
{
    return LH_QtPlugin::init( name, state );
}

int LH_QtPlugin_Mailcount::notify( int code, void *param )
{
    return LH_QtPlugin::notify( code, param );
}

void LH_QtPlugin_Mailcount::term()
{
    LH_QtPlugin::term();
    if( email_addr_ ) delete email_addr_; email_addr_ = 0;
    if( email_days_ ) delete email_days_; email_days_ = 0;
    if( check_interval_ ) delete check_interval_; check_interval_ = 0;
}
