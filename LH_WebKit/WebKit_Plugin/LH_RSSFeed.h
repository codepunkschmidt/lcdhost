#ifndef LH_RSSFEED_H
#define LH_RSSFEED_H

#include <QNetworkAccessManager>
#include <QUrl>
#include <QWidget>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QList>
#include <QHash>

#include "../../LH_Qt_QString.h"
#include "../../LH_Qt_QSlider.h"
#include "../../LH_Qt_InputState.h"
#include "../../LH_Qt_QStringList.h"
#include "../../LH_Qt_int.h"

#include <QObject>

#include "../../LH_QtObject.h"

struct RSSThumbnail
{
    QString url;
    int width;
    int height;
};

struct RSSItem
{
    QString title;
    QString link;
    QString description;
    QString author;
    QString pubDate;
    RSSThumbnail thumbnail;
};

class LH_RSSFeed: public QObject
{
    Q_OBJECT

    QTime notifyTimer_;

    QList<RSSItem> items_;
    int nowshowing_;
    int switchwait_;
    QXmlStreamReader xml_;
    QUrl url_;
    QDateTime lastrefresh_;
    QTime lastopen_;
    QTime lastmove_;

    QNetworkAccessManager *nam_;
    int connectionId;

    void parseXml();

    int refresh_;
    int delay_;

    QList<QObject*> connections_;

public:
    int refresh();
    void setRefresh(int r);
    int delay();
    void setDelay(int d);
    QString url();
    void setUrl(QString u);

    void connect(QObject* intrfce);
    int connections();
    bool release(QObject* intrfce);


    LH_RSSFeed(QString feedUrl, int refreshTime = 5, int delayTime = 5);
    int notify(int code,void* param);

    RSSItem item(int mod = 0);

    void openItem(int mod = 0);
    void moveNext();
    void moveBack();

signals:
    void begin();
    void finished();
    void changed();
    void urlChanged();
    void refreshChanged();
    void delayChanged();

public slots:
    void fetch();
    void finished(QNetworkReply*);
};



extern QHash<QString, LH_RSSFeed* >* rssFeeds;

#endif // LH_RSSFEED_H
