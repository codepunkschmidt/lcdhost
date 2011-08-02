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

#include <QDebug>
#include <QNetworkProxy>

#include "LH_RSSInterface.h"
#include "LH_QtPlugin_WebKit.h"
#include "../WebKitCommand.h"

LH_PLUGIN(LH_QtPlugin_WebKit)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>WebKit</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Allows HTML and JavaScript."
  "</shortdesc>"
  "<longdesc>"
  "<a href=\"http://webkit.org\">WebKit</a> integration. Add URL's or HTML instances, complete with JavaScript."
  "</longdesc>"
"</lcdhostplugin>";

bool LH_QtPlugin_WebKit::startServer()
{
    if( last_start_.isNull() || last_start_.elapsed() > 10000 )
    {
        last_start_ = QTime::currentTime();
        QString wksname = QFileInfo(QCoreApplication::applicationFilePath()).canonicalPath() + "/WebKitServer";
        qDebug() << "LH_WebKit: attempting to start" << wksname;
        QProcess::startDetached( wksname, QStringList("--hidden")
                                << "--datadir"
                                << dir_data()
                                << "--plugindir"
                                << dir_plugins()
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

LH_QtPlugin_WebKit::~LH_QtPlugin_WebKit()
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

const char *LH_QtPlugin_WebKit::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;

    setup_internal_ip_ = new LH_Qt_QString(this, "IP Address (Internal)", "N/A", LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_READONLY);
    setup_internal_ip_->setLink("@/system/IP Address/IPv4/Internal");
    setup_external_ip_ = new LH_Qt_QString(this, "IP Address (External)", "N/A", LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_READONLY);
    setup_external_ip_->setLink("@/system/IP Address/IPv4/External");

    nam_ = new QNetworkAccessManager(this);
    connect(nam_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

    refreshAddresses();
    return 0;
}

void LH_QtPlugin_WebKit::refreshAddresses()
{
    refreshInternalAddress();
    refreshExternalAddress();
}

void LH_QtPlugin_WebKit::refreshExternalAddress()
{
    QUrl url_ = QUrl::fromUserInput( "http://checkip.dyndns.org/" );
    if( url_.isValid() )
    {
        QNetworkProxyQuery npq(url_);
        QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
        if(listOfProxies.count()!=0)
            if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy)
                nam_->setProxy(listOfProxies.at(0));

        nam_->get( QNetworkRequest(url_) );
    }
}

void LH_QtPlugin_WebKit::refreshInternalAddress()
{
    bool found = false;
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if(interface.flags().testFlag(QNetworkInterface::IsLoopBack))
            continue; //loop back address
        if(!interface.flags().testFlag(QNetworkInterface::IsUp))
            continue; //inactive

        foreach(QNetworkAddressEntry entry, interface.addressEntries())
           if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
           {
               found = true;
               setup_internal_ip_->setValue(entry.ip().toString());
           }
    }
    if(!found)
        setup_internal_ip_->setValue("Not connected");
}

void LH_QtPlugin_WebKit::finished( QNetworkReply* reply )
{
    if( reply != NULL )
    {
        if( reply->error() == QNetworkReply::NoError )
        {
            if( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200 )
            {
                QRegExp rx("<body>.*([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})</body>");
                if(rx.indexIn(QString( reply->readAll() )))
                    setup_external_ip_->setValue( rx.cap(1) );
            }
            else
            {
                setup_external_ip_->setValue( QString("Unavailable [%1]").arg( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() ) );
            }
        }
        else
        {
            setup_external_ip_->setValue( QString( "Not connected" ) );
        }
        reply->deleteLater();
    }
    QTimer::singleShot(5000, this, SLOT(refreshAddresses()));
}
