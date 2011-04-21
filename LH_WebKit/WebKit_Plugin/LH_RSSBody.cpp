#include "LH_RSSBody.h"
#include <QDebug>

LH_PLUGIN_CLASS(LH_RSSBody)

lh_class *LH_RSSBody::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Dynamic",
        "DynamicRSSFeedBody",
        "RSS feed (Body)",
        -1, -1,
        lh_instance_calltable_NULL
    };
    return &classInfo;
}

LH_RSSBody::LH_RSSBody(const char *name) : LH_WebKit(name, true)
{
    rss_ = new LH_RSSInterface(this);
    connect( rss_, SIGNAL(changed()), this, SLOT(setRssItem()) );
    connect( rss_, SIGNAL(begin()), this, SLOT(beginFetch()) );

    setup_template_->setHelp(setup_template_->help() + ""
                             "<br><b>\\title</b> : RSS item title "
                             "<br><b>\\author</b> : RSS item author "
                             "<br><b>\\link</b> : RSS item link url "
                             "<br><b>\\pubDate</b> : RSS item published date "
                             "<br><b>\\thumbnail_url</b> : RSS item thumbnail url "
                             "<br><b>\\thumbnail_height</b> : RSS item thumbnail height "
                             "<br><b>\\thumbnail_width</b> : RSS item thumbnail width "
                             );

    setRssItem();
}

int LH_RSSBody::notify(int code,void* param)
{
    return rss_->notify(code,param) | LH_WebKit::notify(code,param);
}

void LH_RSSBody::setRssItem()
{
    sendRequest( QUrl::fromLocalFile( QString::fromUtf8( state()->dir_layout ) + "/" ), rss_->item().description );
}

QString LH_RSSBody::getParsedHtml()
{
    if (setup_parse_->value())
    {
        QString parsedHtml = LH_WebKit::getParsedHtml();
        parsedHtml = parseToken(parsedHtml, "title",            rss_->item().title );
        parsedHtml = parseToken(parsedHtml, "author",           rss_->item().author );
        parsedHtml = parseToken(parsedHtml, "link",             rss_->item().link );
        parsedHtml = parseToken(parsedHtml, "pubDate",          rss_->item().pubDate );
        parsedHtml = parseToken(parsedHtml, "thumbnail_url",    rss_->item().thumbnail.url );
        parsedHtml = parseToken(parsedHtml, "thumbnail_height", QString::number(rss_->item().thumbnail.height) );
        parsedHtml = parseToken(parsedHtml, "thumbnail_width",  QString::number(rss_->item().thumbnail.width) );
        return parsedHtml;
    }
    else
        return html_;
}

void LH_RSSBody::beginFetch()
{
    //setText("Loading RSS feed");
}
