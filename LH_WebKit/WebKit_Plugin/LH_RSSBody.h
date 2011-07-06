#ifndef LH_RSSBODY_H
#define LH_RSSBODY_H

#include "LH_WebKit.h"
#include "../WebKitCommand.h"
#include "LH_RSSInterface.h"

class LH_RSSBody : public LH_WebKit
{
    Q_OBJECT

    LH_RSSInterface rss_;

protected:
    QHash<QString, QString> getTokens();

public:
    LH_RSSBody();

    const char *userInit() { setRssItem(); return 0; }

    int notify(int code,void* param);

    static lh_class *classInfo();

public slots:
    void setRssItem();
    void beginFetch();

};

#endif // LH_RSSBODY_H
