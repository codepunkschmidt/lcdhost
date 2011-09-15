/**
  \file     LH_Mailcount.cpp
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2011, Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <stdlib.h>
#include <QtGlobal>

#include <QCoreApplication>
#include <QtDebug>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QImage>
#include <QPainter>
#include <QRect>
#include <QFont>
#include <QFile>

#include "LH_Mailcount.h"

LH_PLUGIN_CLASS(LH_Mailcount)

lh_class *LH_Mailcount::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "System/Mail",
        "SystemMailIcon",
        "Unread mail (Icon)",
        48,48
    };

    return &classinfo;
}

const char *LH_Mailcount::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;

    envelope_count_ = -1;
    email_count_ = new LH_Qt_int(this,("Unread mail count"),0,
                                 LH_FLAG_READONLY|LH_FLAG_NOSAVE|LH_FLAG_NOSOURCE|LH_FLAG_AUTORENDER);
    email_count_->setLink("/system/mail/count");

    mail_image_ = new LH_Qt_QFileInfo(this,("Mail envelope image"),QFileInfo(),LH_FLAG_NOSOURCE|LH_FLAG_NOSINK|LH_FLAG_AUTORENDER);
    connect( mail_image_, SIGNAL(valueChanged()), this, SLOT(makeEnvelope()) );

    flashing_ = new LH_Qt_bool(this,("Flash when there is mail"),true,LH_FLAG_AUTORENDER);
    connect( flashing_, SIGNAL(valueChanged()), this, SLOT(requestPolling()) );
    connect( flashing_, SIGNAL(valueChanged()), this, SLOT(makeEnvelope()) );

    smoothflash_ = new LH_Qt_bool(this,("Use smooth flashes"),false);
    connect( smoothflash_, SIGNAL(valueChanged()), this, SLOT(requestPolling()) );

    flash_on_ = true;

    return 0;
}

int LH_Mailcount::polling()
{
    if( smoothflash_->value() && flashing_->value() )
    {
        requestRender();
        return 100;
    }
    return 0;
}

int LH_Mailcount::notify(int n,void*p)
{
    if( n & LH_NOTE_SECOND )
    {
        flash_on_ = !flash_on_;
        if( count() != envelope_count_ || (flashing_->value() && envelope_count_) )
            requestRender();
    }
    return LH_QtInstance::notify(n,p)|LH_NOTE_SECOND;
}

void LH_Mailcount::prerender()
{
}

int LH_Mailcount::width(int h)
{
    if( h < 0 ) return 48;
    if( !h ) return 0;
    if( h > 48 ) return 16;;
    return count() ? (h/2) * (count()+1) : h;
}

int LH_Mailcount::height(int w)
{
    return width(w);
}

void LH_Mailcount::makeEnvelope()
{
    int x, y;
    int dx, dy, span;
    QImage stamp;

    if( !image_ || image_->isNull() ) return;

    if( mail_image_->value().isFile() ) stamp.load( mail_image_->value().filePath() );
    else stamp.load( ":/mailcount/images/envelope48.png" );
    stamp = stamp.scaled( image_->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );

    envelope_ = *image_;
    envelope_.fill( QColor( Qt::transparent ).rgba() );

    if( count() < 1 )
    {
        envelope_count_ = count();
        return;
    }

    if( ((qreal)image_->width()/(qreal)stamp.width()) >=
        ((qreal)image_->height()/(qreal)stamp.height()) )
    {
        // left to right
        span = (image_->width() / 2) - (stamp.width() / 2);
        dx = span / count();
        if( dx > stamp.width() / 2 ) dx = stamp.width() / 2;
        if( dx < 1 ) dx = 1;
        x = span - ((dx*(count()-1))/2);
        if( x < 0 ) x = 0;
        y = dy = 0;
    }
    else
    {
        // top to bottom
        span = (image_->height() / 2) - (stamp.height() / 2);
        dy = span / count();
        if( dy > stamp.height() / 2 ) dy = stamp.height() / 2;
        if( dy < 1 ) dy = 1;
        y = span - ((dy*(count()-1))/2);
        if( y < 0 ) y = 0;
        x = dx = 0;
    }

    QPainter painter;
    if( painter.begin(&envelope_) )
    {
        for( int i=0; i<count(); i++ )
        {
            painter.drawImage( x, y, stamp );
            x += dx;
            y += dy;
        }
        envelope_count_ = count();
        painter.end();
    }

    return;
}

QImage *LH_Mailcount::render_qimage( int w, int h )
{
    if( image_ && ( w != image_->width() || h != image_->height() ) )
    {
        delete image_;
        image_ = NULL;
        envelope_count_ = -1;
    }

    if( image_ == NULL )
    {
        image_ = new QImage(w,h,QImage::Format_ARGB32_Premultiplied);
        if( image_ && image_->isNull() )
        {
            delete image_;
            image_ = NULL;
        }
        if( image_ )
        {
            makeEnvelope();
        }
    }

    if( image_ == NULL ) return image_;

    if( count() != envelope_count_ )
    {
        makeEnvelope();
    }

    image_->fill( qRgba(0,0,0,0) );
    if( !count() ) return image_;

    if( !flashing_->value() )
    {
        *image_ = envelope_;
        return image_;
    }

    if( !smoothflash_->value() && flash_on_ ) return image_;

    QPainter painter;
    QImage fadedenv = envelope_;

    if( smoothflash_->value() )
    {
        int ms = QTime::currentTime().msec();
        if( ms < 500 ) fadedenv.fill( QColor(0,0,0,(255*ms)/500).rgba() );
        else fadedenv.fill( QColor(0,0,0,(255*(1000-ms))/500).rgba() );
        if( painter.begin( &fadedenv ) )
        {
            painter.setCompositionMode( QPainter::CompositionMode_SourceIn );
            painter.drawImage(0,0,envelope_);
            painter.end();
        }
    }

    if( painter.begin(image_) )
    {
        painter.setCompositionMode( QPainter::CompositionMode_SourceOver );
        painter.drawImage(
                (image_->width()-fadedenv.width())/2,
                (image_->height()-fadedenv.height())/2,
                fadedenv
                );
        painter.end();
    }

    return image_;
}
