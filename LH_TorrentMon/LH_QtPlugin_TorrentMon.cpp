#include "LH_QtPlugin_TorrentMon.h"

#ifdef WIN32
#include <shlobj.h>
#endif

#include <QDebug>
#include <QNetworkProxyQuery>
#include <QAuthenticator>

#include <math.h>

LH_PLUGIN(LH_QtPlugin_TorrentMon)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Torrent Monitoring</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" STRINGIZE(VERSION) "\nr" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Andy \"Triscopic\" Bridges</author>"
  "<homepageurl><a href=\"http://www.codeleap.co.uk\">CodeLeap</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Allows LCDHost to access data from popular bittorrent clients."
  "</shortdesc>"
  "<longdesc>"
  "This plugin allows users to extract data ..."
  "</longdesc>"
"</lcdhostplugin>";

const char *LH_QtPlugin_TorrentMon::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;

    QFileInfo fi;
#ifdef WIN32
    LPITEMIDLIST pidl;
    char szPath[_MAX_PATH];
    if(SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl) == S_OK && SHGetPathFromIDListA(pidl, szPath))
        fi.setFile(QString("%1/Vuze Downloads/Azureus_Stats.xml").arg(QString(szPath)));
#endif

    setup_use_vuze_  = new LH_Qt_bool(this, "Enable Vuze",false,LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_BLANKTITLE);
    setup_use_vuze_->setTitle("Enable Vuze Monitoring");
    setup_vuze_path_ = new LH_Qt_QFileInfo(this, "Vuze Stats File",fi,LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);

    new LH_Qt_html(this,"<hr>", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK );

    setup_use_webui_ = new LH_Qt_bool(this, "Enable WebUI",false,LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_BLANKTITLE);
    setup_use_webui_->setTitle("Enable µTorrent/BitTorrent Monitoring (via WebUI)");
    setup_webui_username_ = new LH_Qt_QString(this, "WebUI Username", "admin", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);
    setup_webui_password_ = new LH_Qt_QString(this, "WebUI Password", "password", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);
    setup_webui_port_ = new LH_Qt_QString(this, "WebUI Port#", "16335", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDDEN);

    setup_webui_status_ = new LH_Qt_html(this, "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_NOSAVE | LH_FLAG_HIDDEN);
    setup_data_summary_ = new LH_Qt_html(this, "", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_NOSAVE );
    updateSummary();

    reply_ = NULL;
    webUIToken_ = "";
    connect(&nam_, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedWebUI(QNetworkReply*)));
    connect(&nam_, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(authenticationRequiredWebUI(QNetworkReply*,QAuthenticator*)));

    webUIStatuses_.insert(1,"Started");
    webUIStatuses_.insert(2,"Checking");
    webUIStatuses_.insert(4,"Start after check");
    webUIStatuses_.insert(8,"Checked");
    webUIStatuses_.insert(16,"Error");
    webUIStatuses_.insert(32,"Paused");
    webUIStatuses_.insert(64,"Queued");
    webUIStatuses_.insert(128,"Loaded");

    torrents.insert("Vuze", new torrentInfoList());
    torrents.insert("WebUI", new torrentInfoList());

    connect(setup_use_vuze_, SIGNAL(changed()), this, SLOT(changeActiveClient()));
    connect(setup_use_webui_, SIGNAL(changed()), this, SLOT(changeActiveClient()));

    return 0;
}

int LH_QtPlugin_TorrentMon::notify( int n, void *p )
{
    Q_UNUSED(p);
    if(!n || n&LH_NOTE_SECOND)
    {
        if(setup_use_vuze_->value())
            updateVuze();
        else
            torrents.value("Vuze")->clear();
        if(setup_use_webui_->value())
            updateWebUI();
        else
            torrents.value("WebUI")->clear();
        //debugData();
        updateSummary();
    }
    return LH_NOTE_SECOND;
}

void LH_QtPlugin_TorrentMon::updateVuze()
{
    if(setup_vuze_path_->value().exists())
    {
        QFile vuzeFile( setup_vuze_path_->value().canonicalFilePath() );

        QString xmlData;
        if(vuzeFile.open( QIODevice::ReadOnly | QIODevice::Text ))
        {
            QTextStream stream( &vuzeFile );
            xmlData = stream.readAll().remove("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            vuzeFile.close();
        }

        QXmlQuery xmlQuery(QXmlQuery::XQuery10);
        xmlQuery.setFocus(xmlData);
        xmlQuery.setQuery("/STATS/DOWNLOADS/DOWNLOAD");
        if (!xmlQuery.isValid())
            return;
        QXmlResultItems downloadItems;
        xmlQuery.evaluateTo(&downloadItems);
        torrents.value("Vuze")->clear();
        QXmlItem downloadItem(downloadItems.next());
        while (!downloadItem.isNull()) {
            xmlQuery.setFocus(downloadItem);
            torrentInfo torrent;

            torrent.source = "Vuze";
            torrent.name = getXMLValue(xmlQuery, "TORRENT/NAME/text()");
            torrent.size = getXMLValue(xmlQuery, "TORRENT/SIZE/TEXT/text()");

            torrent.progress = getXMLValue(xmlQuery, "COMPLETED/text()").toInt() / 10.0;
            torrent.ratio = getXMLValue(xmlQuery, "SHARE_RATIO/text()").toInt() / 1000.0;
            torrent.status = getXMLValue(xmlQuery, "DOWNLOAD_STATUS/text()");
            torrent.seeds = getXMLValue(xmlQuery, "TOTAL_SEEDS/text()").toInt();
            torrent.peers = getXMLValue(xmlQuery, "TOTAL_LEECHERS/text()").toInt();
            torrent.downSpeed = getXMLValue(xmlQuery, "DOWNLOAD_SPEED/TEXT/text()");
            torrent.upSpeed = getXMLValue(xmlQuery, "UPLOAD_SPEED/TEXT/text()");
            torrent.eta = getXMLValue(xmlQuery, "ETA/text()");

            torrents.value("Vuze")->append(torrent);
            downloadItem = downloadItems.next();
        }
        if (downloadItems.hasError())
            qWarning() << "LH_TorrentMon: Error parsing Vuze XML.";
    }
}

QString LH_QtPlugin_TorrentMon::getXMLValue(QXmlQuery xmlQuery, QString path)
{
    QString result = "";
    xmlQuery.setQuery(path);
    if (xmlQuery.isValid())
    {
        xmlQuery.evaluateTo(&result);
        result.remove("\n");
    }
    return result;
}

void LH_QtPlugin_TorrentMon::updateWebUI()
{

    if(reply_!=NULL)
        if(reply_->isRunning())
            return;

    QString url = QString("http://127.0.0.1:%1/gui/").arg(setup_webui_port_->value());
    if(webUIToken_=="")
        url += "token.html";

    QNetworkProxyQuery npq(QUrl::fromUserInput(url));
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    if(listOfProxies.count()!=0)
        if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy)
            nam_.setProxy(listOfProxies.at(0));

    reply_ = nam_.get( QNetworkRequest(url+QString("?list=1&token=%1").arg(webUIToken_)) );
}

void LH_QtPlugin_TorrentMon::debugData()
{
    foreach (QString key, torrents.keys())
    {
        foreach (torrentInfo torrent, *torrents.value(key))
        {
            qDebug() << torrent.name <<"; "<< torrent.source <<"; "<< torrent.size <<"; " << QString::number(torrent.progress,'f',3) << "%; ";
            qDebug() << torrent.status <<"; "<< torrent.seeds <<"; " << torrent.peers << "; ";
            qDebug() << torrent.downSpeed <<"; "<< torrent.upSpeed <<"; " << torrent.eta << "; ";
        }
    }
}

void LH_QtPlugin_TorrentMon::finishedWebUI(QNetworkReply* reply)
{
    if( reply )
    {
        if( reply->error() == QNetworkReply::NoError && reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200 )
        {
            setup_webui_status_->setVisible(false);
            QString response = QString(reply->readAll());
            if(response.startsWith("<html>"))
            {
                QRegExp rx("<html>.*<div id='token'[^>]*>(.*)</div>.*</html>");
                rx.setMinimal(true);
                if (rx.indexIn(response)!=-1)
                    webUIToken_ = rx.cap(1);
            } else {
                bool ok;
                QVariantMap result = Json::parse(response, ok).toMap();
                if(ok) {
                    torrents.value("WebUI")->clear();
                    foreach(QVariant torrent, result["torrents"].toList()) {
                        QList<QVariant> torrentDetails = torrent.toList();

                        torrentInfo torrent;

                        torrent.source = "WebUI";
                        torrent.name = torrentDetails[2].toString();
                        torrent.size = toSizeString(torrentDetails[3].toLongLong());

                        torrent.progress = torrentDetails[4].toInt() / 10.0;
                        torrent.ratio = torrentDetails[7].toInt() / 1000.0;
                        torrent.status = toWebUIStatusString(torrentDetails[1].toInt(), torrentDetails[4].toInt());
                        torrent.seeds = torrentDetails[14].toInt();
                        //torrent.seeds.swarm = torrentDetails[15].toInt();
                        torrent.peers = torrentDetails[12].toInt();
                        //torrent.peers.swarm = torrentDetails[13].toInt();
                        torrent.downSpeed = toSizeString(torrentDetails[9].toLongLong())+"/s";
                        torrent.upSpeed = toSizeString(torrentDetails[8].toLongLong())+"/s";
                        torrent.eta = torrentDetails[10].toString();
                        if(torrent.eta == "-1")
                            torrent.eta = "Finished";
                        else
                        if(torrent.eta == "0")
                            torrent.eta = "";
                        else
                            torrent.eta = toTimeString(torrentDetails[10].toLongLong());
                        torrents.value("WebUI")->append(torrent);
                    }
                }
                else
                    qWarning() << "LH_TorrentMon: An error occurred during parsing WebUI torrent data.";
            }
        }
        else
        {
            switch(reply->error())
            {
                case QNetworkReply::NoError:
                    setup_webui_status_->setHelp("Unable to aqcuire WebUI data: Http code " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
                    break;
                case QNetworkReply::ConnectionRefusedError:
                    setup_webui_status_->setHelp("Connection refused. <br/>Either the uTorrent/BitTorrent is not running or the settings are wrong.");
                    break;
                default:
                    setup_webui_status_->setHelp("Unable to aqcuire WebUI data: " + reply->errorString());
                    break;
            }
            setup_webui_status_->setHidden( false);
        }
        reply->deleteLater();
        reply = NULL;
    }
    reply_->deleteLater();
    reply_ = NULL;
}

void LH_QtPlugin_TorrentMon::authenticationRequiredWebUI(QNetworkReply *reply, QAuthenticator *auth)
{
    Q_UNUSED(reply);
    auth->setUser(setup_webui_username_->value());
    auth->setPassword(setup_webui_password_->value());
}

QString LH_QtPlugin_TorrentMon::toSizeString(qlonglong bytes)
{
    QString units = "B";
    float result = bytes;

    while (result >= 1024 && units!="TB")
    {
        result /= 1024;
        if(units=="GB") units = "TB";
        if(units=="MB") units = "GB";
        if(units=="KB") units = "MB";
        if(units=="B") units = "KB";
    }

    QString numberString;
    numberString.sprintf("%.2f", result);
    return QString("%1 %2").arg(numberString).arg(units);
}

QString LH_QtPlugin_TorrentMon::toTimeString(qlonglong etaSeconds)
{
    if (etaSeconds / (7*24*60*60) >=52)
    {
        int years = etaSeconds / (52*7*24*60*60);
        int weeks = floor((etaSeconds % (52*7*24*60*60)) / (7.0*24*60*60) + 0.5);
        return QString("%1y %2w").arg(years).arg(weeks);
    }
    if (etaSeconds / (24*60*60) >=7)
    {
        int weeks = etaSeconds / (7*24*60*60);
        int days = floor((etaSeconds % (7*24*60*60)) / (24.0*60*60) + 0.5);
        return QString("%1w %2d").arg(weeks).arg(days);
    }
    if (etaSeconds / (60*60) >=24)
    {
        int days = etaSeconds / (24*60*60);
        int hours = floor((etaSeconds % (24*60*60)) / (60.0*60) + 0.5);
        return QString("%1d %2h").arg(days).arg(hours);
    }
    if (etaSeconds / (60) >=60)
    {
        int hours = etaSeconds / (60*60);
        int minutes = floor((etaSeconds % (60*60)) / (60.0) + 0.5);
        return QString("%1h %2m").arg(hours).arg(minutes);
    }
    if (etaSeconds / (1) >=60)
    {
        int minutes = etaSeconds / (60);
        int seconds = floor((etaSeconds % (60)) / (1.0) + 0.5);
        return QString("%1m %2s").arg(minutes).arg(seconds);
    }
    if (etaSeconds >=0)
    {
        int seconds = etaSeconds;
        return QString("%1s").arg(seconds);
    }
    return "";
}

QString LH_QtPlugin_TorrentMon::toWebUIStatusString(int statusCode, int perMilleProgress)
{
    QString statusString = "";
    foreach (int key, webUIStatuses_.keys())
    {
        if( (key & statusCode) == key )
            statusString = QString("%1 [%2]").arg(statusString).arg(webUIStatuses_.value(key));
    }
    //http://forum.utorrent.com/viewtopic.php?pid=272592#p272592
    //http://forum.utorrent.com/viewtopic.php?id=50779
    if (statusString.contains("[Error]"))
        return "Error";
    if (statusString.contains("[Paused]"))
        return "Paused";
    if (perMilleProgress<1000)
    {
        if (statusString.contains("[Started]"))
            return "Downloading";
        if (statusString.contains("[Queued]"))
            return "Queued";
        if (statusString.contains("[Checking]"))
            return "Checking";
        return "Stopped";
    }
    else
    {
        if (statusString.contains("[Started]"))
            return "Seeding";
        if (statusString.contains("[Loaded]"))
            return "Queued Seed";
        return "Finished";
    }
    //return QString("%1 (%2)").arg(statusString.trimmed()).arg(statusCode);
}

void LH_QtPlugin_TorrentMon::changeActiveClient()
{
    setup_vuze_path_->setVisible(setup_use_vuze_->value());
    setup_webui_username_->setVisible(setup_use_webui_->value());
    setup_webui_password_->setVisible(setup_use_webui_->value());
    setup_webui_port_->setVisible(setup_use_webui_->value());
}

void LH_QtPlugin_TorrentMon::updateSummary()
{
    QString summaryData = "<hr>"
            "<style type='text/css'>"
            "   table.main{border-color:black; border-style:solid; border-width:1px;}"
            "   td.gray  {padding:2px; border-width:1px; border-style:solid; border-color:black; background-color: #E0E0E0;}"
            "   td.blue  {padding:2px; border-width:1px; border-style:solid; border-color:black; background-color: #D8D8F0;}"
            "   td.green {padding:2px; border-width:1px; border-style:solid; border-color:black; background-color: #D8F0D8;}"
            "   td.yellow{padding:2px; border-width:1px; border-style:solid; border-color:black; background-color: #F8F8E0;}"
            "</style>"
            "<table cellspacing='1' class='main'>";

    foreach (QString key, torrents.keys())
    {
        foreach (torrentInfo torrent, *torrents.value(key))
        {
            summaryData += QString("<tr><td colspan='7' class='yellow'>%1</td><td class='yellow' colspan='2'>%5</td><td class='yellow'>%4</td></tr>"
                                   "<tr><td> </td><td class='%8'>%2</td><td class='gray'>%3</td>"
                                   "<td class='gray'>%6</td><td class='gray'>%7</td>")
                    .arg(torrent.name)
                    .arg(torrent.source)
                    .arg(torrent.size)
                    .arg(QString::number(torrent.progress,'f',0) + "%")
                    .arg(torrent.status) //5
                    .arg(torrent.seeds)
                    .arg(torrent.peers)
                    .arg(torrent.source=="Vuze"? "blue" : "green");


            summaryData += QString("<td class='gray' colspan='2'>%1</td><td class='gray' colspan='2'>%2</td><td class='gray'>%3</td></tr>")
                    .arg(torrent.downSpeed)
                    //.arg(torrent.upSpeed)
                    .arg(torrent.eta)
                    .arg(torrent.ratio);

        }
    }

    summaryData += "</table>";
    setup_data_summary_->setHelp(summaryData);
}
