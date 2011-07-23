/**
  \file     LH_QtPlugin_TS3.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh.

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

#include "LH_QtPlugin_TS3.h"

LH_PLUGIN(LH_QtPlugin_TS3)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>TS3</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" STRINGIZE(VERSION) "\nr" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Andy \"Triscopic\" Bridges</author>"
  "<homepageurl><a href=\"http://www.codeleap.co.uk\">CodeLeap</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "TS3..."
  "</shortdesc>"
  "<longdesc>"
    "<p>TS3...</p>"
"</longdesc>"
"</lcdhostplugin>";

const char *LH_QtPlugin_TS3::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;

    server_action_ = sa_disconnected;
    tryConnectTimer_.start();

    socket_ = new QTcpSocket(this);

    setup_status = new LH_Qt_QString(this,tr("Connection Status"),QString(),LH_FLAG_FIRST | LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | LH_FLAG_HIDETITLE, lh_type_string_htmlhelp );
    updateStatus(false);

    setup_speakers = new LH_Qt_QString(this, "Speakers", "", LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSINK);
    setup_speakers->setLink("@/Monitoring/3rdParty/TeamSpeak3/Speaking");

    connect(socket_, SIGNAL(connected()), this, SLOT(TS3Connected()));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(TS3Disconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(TS3DataReceived()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(TS3ConnectionError(QAbstractSocket::SocketError)));

    openConnection();
    return 0;
}

int LH_QtPlugin_TS3::notify( int code, void *param )
{
    if( code & LH_NOTE_SECOND && server_action_ == sa_disconnected  && tryConnectTimer_.elapsed() >= 5000)
        openConnection();
    return LH_NOTE_SECOND;
}

void LH_QtPlugin_TS3::openConnection()
{
    switch(socket_->state())
    {
    case QAbstractSocket::UnconnectedState:
        //qDebug() << "LH_TS3: Attempting to connect.";
        server_action_ = sa_connecting;
        socket_->connectToHost("127.0.0.1", 25639);
        break;
    case QAbstractSocket::ConnectedState:
        //qWarning() << "LH_TS3: Cannot connect - already connected.";
        server_action_ = sa_reconnect;
        socket_->disconnectFromHost();
        break;
    case QAbstractSocket::ClosingState:
        //qWarning() << "LH_TS3: Cannot connect - still closing previous connection.";
        server_action_ = sa_reconnect;
        break;
    default:
        //qWarning() << "LH_TS3: Cannot connect - previous connection attempt was in progress - aborting.";
        server_action_ = sa_reconnect;
        socket_->abort();
    }
}

void LH_QtPlugin_TS3::TS3Connected() {
    //qDebug() << "LH_TS3: Connected";
    updateStatus(true);

    server_action_ = sa_eventregister_pending;
    sendMessage(QString("clientnotifyregister schandlerid=%1 event=notifytalkstatuschange \n"

                        "clientnotifyregister schandlerid=%1 event=notifyclientleftview \n"
                        "clientnotifyregister schandlerid=%1 event=notifycliententerview \n"
                        "clientnotifyregister schandlerid=%1 event=notifyclientupdated \n"
                        "clientnotifyregister schandlerid=%1 event=notifyclientmoved \n"

                        "clientnotifyregister schandlerid=%1 event=notifychannelcreated \n"
                        "clientnotifyregister schandlerid=%1 event=notifychanneledited \n"
                        "clientnotifyregister schandlerid=%1 event=notifychanneldeleted \n"
                        "clientnotifyregister schandlerid=%1 event=notifychannelmoved \n"
                        ).arg(0));
}

void LH_QtPlugin_TS3::TS3Disconnected()
{
    //qDebug() << "LH_TS3: Disconnected";
    updateStatus(false);
    server_action_ = sa_disconnected;
    if(server_action_ == sa_reconnect)
        openConnection();
    else
        tryConnectTimer_.restart();
}

void LH_QtPlugin_TS3::TS3ConnectionError(QAbstractSocket::SocketError socketError)
{
    if(socketError == QAbstractSocket::ConnectionRefusedError)
    {
        server_action_ = sa_disconnected;
        tryConnectTimer_.restart();
        //qDebug() << "LH_TS3: Connection Refused - will try again in a few moments";
    }
    else
    {
        qWarning() << "LH_TS3: Connection Error - " << socket_->errorString();
        server_action_ = sa_disconnected;
        tryConnectTimer_.restart();
    }
}

responseResult LH_QtPlugin_TS3::parseResult(QString msg)
{
    QRegExp rx("error id=(\\S+) msg=(\\S+)(?: extra_msg=(\\S+)){0,1}");
    if(rx.indexIn(msg)==-1)
        return (responseResult){false, false, 0, "", ""};
    else
        return (responseResult){true, (rx.cap(1)=="0"), (rx.cap(1).toInt()), rx.cap(2).replace("\\s"," "), rx.cap(3).replace("\\s"," ")};
}

void LH_QtPlugin_TS3::TS3DataReceived()
{
    QString receivedMsg = QString(socket_->readAll());
    //QRegExp rxSCHandler("selected schandlerid=([0-9]*)");
    QRegExp rxNotify("notify(\\w*) schandlerid=([0-9]*) (.*)");
    responseResult result = parseResult(receivedMsg);

    if(result.isResult && !result.isValid)
    {
        if(result.errorCode == 1794)
        {
            channels_.clear();
            clients_.clear();
            speakers_.clear();
            refreshSpeakers();
            //qDebug() << "LH_TS3: Client is not connected to a server.";
            updateStatus(true);
        }
        else
        {
            qWarning() << QString("LH_TS3: Unhandled Response Error: %1%2 (%3)").arg(result.errorMessage, (result.extraMessage==""?"":QString(" (%1)").arg(result.extraMessage))).arg(result.errorCode);
            Q_ASSERT(false);
        }
    }
    else if(rxNotify.indexIn(receivedMsg)!=-1)
    {
        if(rxNotify.cap(1)=="talkstatuschange")
            talkChanged(rxNotify.cap(3));
        if(QString("clientleftview,cliententerview,clientupdated,clientmoved").split(',').contains(rxNotify.cap(1)) ||
           QString("channelcreated,channeledited,channeldeleted,channelmoved").split(',').contains(rxNotify.cap(1)) )
        {
            server_action_ = sa_channellist_pending;
            sendMessage("channellist");
        }
    }
    else switch(server_action_)
    {
    case sa_eventregister_pending:
        if(!result.isResult)
            break;
        server_action_ = sa_channellist_pending;
        sendMessage("channellist");
        break;
    case sa_channellist_pending:
        if(channels_.load(receivedMsg))
        {
            updateStatus(true,true,true);
            //qDebug()<<"LH_TS3: Server has " << channels_.length() << " channel(s)";
            server_action_ = sa_clientlist_pending;
            sendMessage("clientlist -voice");
        }
        break;
    case sa_clientlist_pending:
        if(clients_.load(receivedMsg))
        {
            updateStatus(true,true,true,true);
            server_action_ = sa_listening;
        }
        break;
    case sa_none:
    case sa_disconnected:
    case sa_connecting:
    case sa_reconnect:
    case sa_listening:
        break;
    }
}

int LH_QtPlugin_TS3::sendMessage(QString msg)
{
    //qDebug() << QString(msg).toUtf8();
    return socket_->write(QString("%1\n").arg(msg).toUtf8());
}

void LH_QtPlugin_TS3::talkChanged(QString params)
{
    QRegExp rx("status=([0-9]*) isreceivedwhisper=([0-9]*) clid=([0-9]*)");

    bool status = false;
    bool whisper = false;
    int clid = -1;

    if(rx.indexIn(params)!=-1)
    {
        status = (rx.cap(1).toInt()!=0);
        whisper = (rx.cap(2).toInt()!=0);
        clid = rx.cap(3).toInt();
    }

    if(status)
    {
        if(speakers_.contains(clid))
            return;
        speakers_.insert(clid, clients_.value(clid));
    } else {
        if(!speakers_.contains(clid))
            return;
        speakers_.remove(clid);
    }
    refreshSpeakers();
}

void LH_QtPlugin_TS3::refreshSpeakers()
{
    QString speakers = speakers_.toString();
    if(setup_speakers->value()!=speakers)
    {
        //qDebug() << "Speakers: " << speakers;
        setup_speakers->setValue(speakers);
        emit speakersChanged(speakers);
    }
}

void LH_QtPlugin_TS3::updateStatus(bool isRunning, bool isConnected, bool showChannels, bool showClients)
{
    setup_status->setHelp(QString("<table><tr><td><img src=':/images/%5.png'/></td><td>TS3 is %1%2.</td></tr></table><hr/>Channels: %3<br/>Users: %4<hr/>")
            .arg(isRunning? "running" : "not running")
            .arg(isRunning? (isConnected? " and is connected" : " but is not connected") : "")
            .arg(showChannels? QString::number(channels_.count()) : "N/A")
            .arg(showClients? QString::number(clients_.count()) : "N/A")
            .arg(!isRunning? "notrunning" : (isConnected? "active" : "unconnected"))
            );
}
