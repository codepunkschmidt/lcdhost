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

    setRssItem();
}

int LH_RSSBody::notify(int code,void* param)
{
    return rss_->notify(code,param) | LH_WebKit::notify(code,param);
}

void LH_RSSBody::setRssItem()
{
    sendRequest( QUrl::fromLocalFile( QString::fromUtf8( state()->dir_layout ) + "/" ), rss_->item().description );
    //if( setText(rss_->item().title) ) requestRender();
}

/*QImage *LH_RSSBody::render_qimage( int w, int h )
{
    if( !prepareForRender(w,h) ) return NULL;

    QPainter painter;
    if( painter.begin(image_) )
    {
        QRectF target;

        target.setSize( textimage().size() );
        target.moveLeft( image_->width()/2 - textimage().width()/2 );
        target.moveTop( image_->height()/2 - textimage().height()/2 );

        if( textimage().width() > image_->width() )
            target.moveLeft( 0 );

        painter.drawImage( target, textimage(), textimage().rect() );
        painter.end();
    }

    return image_;
}*/

void LH_RSSBody::beginFetch()
{
    //setText("Loading RSS feed");
}
