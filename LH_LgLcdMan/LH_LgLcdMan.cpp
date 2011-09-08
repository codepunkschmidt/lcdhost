/**
  \file     LH_LgLcdMan.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <QDebug>
#include <QCoreApplication>
#include <QEvent>
#include <QString>
#include <QFile>

#include "LH_LgLcdMan.h"
#include "LH_LgLcdLegacyThread.h"
#include "LH_LgLcdCallbackThread.h"

#ifdef Q_WS_WIN
# ifndef UNICODE
#  error ("This isn't going to work")
# endif
# include "windows.h"
# include "wow64.h"
# include "win/lglcd.h"
#endif

#ifdef Q_WS_MAC
# include "mac/lgLcdError.h"
# include "mac/lgLcd.h"
# ifndef ERROR_FILE_NOT_FOUND
#  define ERROR_FILE_NOT_FOUND 2
# endif
#endif

LH_PLUGIN(LH_LgLcdMan) =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>LgLcdMan</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Logitech G-series keyboard driver using the Logitech software."
  "</shortdesc>"
  "<longdesc>"
  "This driver interfaces with the Logitech LCD Manager, which is part of the Logitech G-series keyboard driver software."
  "</longdesc>"
"</lcdhostplugin>";

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

const char *LH_LgLcdMan::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;
#ifdef Q_WS_WIN
    thread_ = new LH_LgLcdCallbackThread(this);

    // connected, log logitech version
    HKEY hKey = (HKEY)0;
    const char *logitechkey;

    logitechkey = "SOFTWARE\\Logitech\\Logitech Gaming Software\\ConfigMgr";
    if( isWoW64() ) RegOpenKeyExA( HKEY_CURRENT_USER, logitechkey, 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hKey );
    if( hKey == (HKEY)0 ) RegOpenKeyExA( HKEY_CURRENT_USER, logitechkey, 0, KEY_QUERY_VALUE, &hKey );
    if( hKey != (HKEY)0 )
    {
        char logitech_version[32];
        DWORD len;
        DWORD type;
        len = sizeof(logitech_version)-1;
        if( RegQueryValueExA( hKey, "RepositoryVersion", NULL, &type, (LPBYTE) logitech_version, &len ) == ERROR_SUCCESS )
        {
            logitech_version[len] = '\0';
            qDebug() << "Logitech Gaming Software version" << logitech_version;
        }
    }

    logitechkey = "SOFTWARE\\Logitech\\LCD Software\\LCD Manager\\CurrentVersion";
    if( isWoW64() ) RegOpenKeyExA( HKEY_LOCAL_MACHINE, logitechkey, 0, KEY_QUERY_VALUE|KEY_WOW64_64KEY, &hKey );
    if( hKey == (HKEY)0 ) RegOpenKeyExA( HKEY_LOCAL_MACHINE, logitechkey, 0, KEY_QUERY_VALUE, &hKey );
    if( hKey != (HKEY)0 )
    {
        char logitech_version[32];
        DWORD len;
        DWORD type;
        len = sizeof(logitech_version)-1;
        if( RegQueryValueExA( hKey, "Version", NULL, &type, (LPBYTE) logitech_version, &len ) == ERROR_SUCCESS )
        {
            logitech_version[len] = '\0';
            if( logitech_version[0] < '3' )
                qWarning() << "Required version of Logitech software is 3.00 or above, you have" << logitech_version;
            else
                qDebug() << "Logitech software version" << logitech_version;
        }
    }
#endif
#ifdef Q_WS_MAC
    thread_ = new LH_LgLcdLegacyThread( this );
#endif

    if( thread_ )
        thread_->start();

    return NULL;
}

LH_LgLcdMan::~LH_LgLcdMan()
{
    if( thread_ )
    {
        thread_->quit();
        if( !thread_->wait(2500) )
        {
            qDebug() << "LH_LgLcdMan: Logitech driver thread not responding, terminating it";
            thread_->terminate();
        }
        if( !thread_->wait(1000) )
        {
            qDebug() << "LH_LgLcdMan: Logitech driver thread won't terminate";
        }
        else
            delete thread_;
    }
    return;
}

