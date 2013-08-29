#ifndef LH_RSSTEXT_H
#define LH_RSSTEXT_H

#include "../LH_Text/LH_Text.h"
#include "LH_RSSInterface.h"

class LH_RSSText : public LH_Text
{
    Q_OBJECT

    LH_RSSInterface* rss_;

public:
    LH_RSSText();

    const char *userInit();
    int notify(int code,void* param);
    QImage *render_qimage( int w, int h );

    static lh_class *classInfo();

public slots:
    void setRssItem();
    void beginFetch();

};

#endif // LH_RSSTEXT_H
