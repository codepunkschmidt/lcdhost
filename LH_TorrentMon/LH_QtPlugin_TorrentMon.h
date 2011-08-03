/**
  \file     LH_QtPlugin_Text.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009 Johan Lindh

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#ifndef LH_QTPLUGIN_TORRENTMON_H
#define LH_QTPLUGIN_TORRENTMON_H

#include "../LH_QtPlugin.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QFileInfo.h"

#include "../json.h"

#include <QXmlQuery>
#include <QXmlResultItems>

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHash>

#define VERSION 1.00

struct connectedSwarm {
    int connected;
    int swarm;
};

struct torrentInfo {
    QString source;
    QString name;
    QString size;
    float progress;
    float ratio;
    QString status;
    int seeds;
    int peers;
    QString downSpeed;
    QString upSpeed;
    QString eta;
};

typedef QList<torrentInfo> torrentInfoList;

class LH_QtPlugin_TorrentMon : public LH_QtPlugin
{
    Q_OBJECT

    void updateVuze();
    void updateWebUI();
    QString getXMLValue(QXmlQuery xmlQuery, QString path);
    void debugData();
    QString toSizeString(qlonglong);
    QString toTimeString(qlonglong);
    QString toWebUIStatusString(int,int);

    void updateSummary();

    QHash<QString, torrentInfoList* > torrents;

    QNetworkReply* reply_;
    QString webUIToken_;

    QNetworkAccessManager nam_;
    QHash<int,QString> webUIStatuses_;
protected:
    LH_Qt_bool *setup_use_vuze_;
    LH_Qt_QFileInfo *setup_vuze_path_;
    LH_Qt_bool *setup_use_webui_;
    LH_Qt_QString *setup_webui_username_;
    LH_Qt_QString *setup_webui_password_;
    LH_Qt_QString *setup_webui_port_;

    LH_Qt_QString *setup_data_summary_;

public:
    const char *userInit();
    int notify( int code, void *param );

public slots:
    void finishedWebUI(QNetworkReply*);
    void authenticationRequiredWebUI(QNetworkReply*,QAuthenticator*);

    void changeActiveClient();
};

#endif // LH_QTPLUGIN_TORRENTMON_H
