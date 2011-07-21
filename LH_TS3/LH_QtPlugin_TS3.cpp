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

    server_action_ = sa_none;

    pollTimer_.start();

    socket_ = new QTcpSocket(this);

    setup_speakers = new LH_Qt_QString(this, "Speakers", "", LH_FLAG_READONLY | LH_FLAG_NOSAVE | LH_FLAG_NOSINK);
    setup_speakers->setLink("@/monitoring/3rdParty/TS3/Speaking");

    connect(socket_, SIGNAL(connected()), this, SLOT(TS3Connected()));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(TS3Disconnected()));
    connect(socket_, SIGNAL(readyRead()), this, SLOT(TS3DataReceived()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(TS3ConnectionError(QAbstractSocket::SocketError)));

    openConnection();
    return 0;
}

void LH_QtPlugin_TS3::openConnection()
{
    switch(socket_->state())
    {
    case QAbstractSocket::UnconnectedState:
        qDebug() << "LH_TS3: Attempting to connect.";
        socket_->connectToHost("127.0.0.1", 25639);
        break;
    case QAbstractSocket::ConnectedState:
        qWarning() << "LH_TS3: Cannot connect - already connected.";
        server_action_ = sa_reconnect;
        socket_->disconnectFromHost();
        break;
    case QAbstractSocket::ClosingState:
        qWarning() << "LH_TS3: Cannot connect - still closing previous connection.";
        server_action_ = sa_reconnect;
        break;
    default:
        qWarning() << "LH_TS3: Cannot connect - previous connection attempt was in progress - aborting.";
        server_action_ = sa_reconnect;
        socket_->abort();
    }
}

void LH_QtPlugin_TS3::TS3Connected() {
    qDebug() << "LH_TS3: Connected";
    server_action_ = sa_channellist_pending;
    sendMessage("channellist");
}

void LH_QtPlugin_TS3::TS3Disconnected()
{
    qDebug() << "LH_TS3: Disconnected";
    if(server_action_ == sa_reconnect)
    {
        server_action_ = sa_none;
        openConnection();
    }
}

void LH_QtPlugin_TS3::TS3DataReceived()
{
    QString receivedMsg = QString(socket_->readAll());
    QRegExp rxResult("error id=(\\S+) msg=(\\S+)(?: extra_msg=(\\S+)){0,1}");
    QRegExp rxSCHandler("selected schandlerid=([0-9]*)");
    QRegExp rxNoteTalkChange("notifytalkstatuschange schandlerid=([0-9]*) status=([0-9]*) isreceivedwhisper=([0-9]*) clid=([0-9]*)");
    responseResult result = (rxResult.indexIn(receivedMsg)==-1? (responseResult){false, false, 0, "", ""} : (responseResult){true, (rxResult.cap(1)=="0"), (rxResult.cap(1).toInt()), rxResult.cap(2).replace("\\s"," "), rxResult.cap(3).replace("\\s"," ")});

    if(result.isResult && !result.isValid)
        qWarning() << QString("LH_TS3: Error: %1%2").arg(result.errorMessage, (result.extraMessage==""?"":QString(" (%1)").arg(result.extraMessage)));
    else if(rxSCHandler.indexIn(receivedMsg)!=-1)
        schandlerid_ = rxSCHandler.cap(1).toInt();
    else if(rxNoteTalkChange.indexIn(receivedMsg)!=-1)
        talkChanged((rxNoteTalkChange.cap(2).toInt()!=0),(rxNoteTalkChange.cap(3).toInt()!=0),rxNoteTalkChange.cap(4).toInt());
    else switch(server_action_)
    {
    case sa_channellist_pending:
        if(channels_.load(receivedMsg))
        {
            //qDebug()<<"LH_TS3: Server has " << channels_.length() << " channel(s)";
            server_action_ = sa_clientlist_pending;
            sendMessage("clientlist -voice");
        }
        break;
    case sa_clientlist_pending:
        if(clients_.load(receivedMsg))
        {
            server_action_ = sa_listening;
            sendMessage(QString("clientnotifyregister schandlerid=%1 event=notifytalkstatuschange").arg(schandlerid_));
        }
        break;
    case sa_none:
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

void LH_QtPlugin_TS3::talkChanged(bool status, bool whisper, int clid)
{
    Q_UNUSED(whisper);
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
    QString newSpeakers = speakers_.toString();
    //qDebug() << "Speakers: " << newSpeakers;
    setup_speakers->setValue(newSpeakers);
    emit speakersChanged(newSpeakers);
}

