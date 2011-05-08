#include "LH_RSSText.h"
#include <QPainter>
#include <QDebug>

LH_PLUGIN_CLASS(LH_RSSText)

lh_class *LH_RSSText::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Dynamic",
        "DynamicRSSFeedText",
        "RSS feed (Text)",
        -1, -1,
        lh_instance_calltable_NULL
    };

    if( classInfo.width == -1 )
    {
        QFont arial10("Arial",10);
        QFontMetrics fm( arial10 );
        classInfo.height = fm.height();
        classInfo.width = classInfo.height * 4;
    }

    return &classInfo;
}

LH_RSSText::LH_RSSText(const char *name) : LH_Text(name), rss_(this)
{

    connect( &rss_, SIGNAL(changed()), this, SLOT(setRssItem()) );
    connect( &rss_, SIGNAL(begin()), this, SLOT(beginFetch()) );

    setup_horizontal_->setFlag( LH_FLAG_HIDDEN, true );
    setup_vertical_->setFlag( LH_FLAG_HIDDEN, true );
    setup_scrollrate_->setFlag( LH_FLAG_HIDDEN, true );
    setup_scrollstep_->setFlag( LH_FLAG_HIDDEN, true );
    setup_text_->setFlag( LH_FLAG_HIDDEN, true );
    setup_text_->setFlag( LH_FLAG_NOSAVE, true );
    setup_text_->setFlag( LH_FLAG_READONLY, true );

    setRssItem();
}

int LH_RSSText::notify(int code,void* param)
{
    return rss_.notify(code,param) | LH_Text::notify(code,param);
}

void LH_RSSText::setRssItem()
{
    if( setText(rss_.item().title) ) requestRender();
}


QImage *LH_RSSText::render_qimage( int w, int h )
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
}


void LH_RSSText::beginFetch()
{
    setText("Loading RSS feed");
}
