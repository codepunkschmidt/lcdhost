/**
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2010 Johan Lindh

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

#include <QProcess>
#include <QtNetwork>
#include <QFileInfo>

#include "LH_RSSInterface.h"
#include "LH_QtPlugin_WebKit.h"
#include "../WebKitCommand.h"

LH_PLUGIN(LH_QtPlugin_WebKit);
lh_buildinfo buildinfo = LH_STD_BUILDINFO;

const char *LH_QtPlugin_WebKit::lh_load()
{
    return NULL;
}

bool LH_QtPlugin_WebKit::startServer()
{
    if( last_start_.isNull() || last_start_.elapsed() > 10000 )
    {
        const char *dir_data_p = 0;
        const char *dir_plugins_p = 0;
        callback( lh_cb_dir_data, &dir_data_p );
        callback( lh_cb_dir_plugins, &dir_plugins_p );
        last_start_ = QTime::currentTime();
        QString wksname = QFileInfo(QCoreApplication::applicationFilePath()).canonicalPath() + "/WebKitServer";
        qDebug() << "LH_WebKit: attempting to start" << wksname;
        QProcess::startDetached( wksname, QStringList("--hidden")
                                << "--datadir"
                                << QString( dir_data_p )
                                << "--plugindir"
                                << QString( dir_plugins_p )
                                << "--verbose"
                                );
    }
    return true;
}

bool LH_QtPlugin_WebKit::sendQuit()
{
    QLocalSocket sock;
    sock.connectToServer("LCDHost_WebKitServer");
    if( !sock.waitForConnected(100) ) return false;
    WebKitCommand('Q').write(&sock);
    sock.waitForBytesWritten(100);
    sock.disconnectFromServer();
    return true;
}

void LH_QtPlugin_WebKit::lh_unload()
{
    sendQuit();

    if(rssFeeds != NULL)
    {
        while( !rssFeeds->isEmpty() )
        {
            QString url = rssFeeds->keys().at(0);
            LH_RSSFeed* feed = rssFeeds->take(url);
            if( feed ) delete feed;
        }
        delete rssFeeds;
        rssFeeds = NULL;
    }
}
