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

#ifdef Q_WS_WIN
#include <windows.h>
/* Make sure unread mail function is declared */
typedef HRESULT (WINAPI *SHGetUnreadMailCountW_t)(
    HKEY hKeyUser,
    LPCTSTR pszMailAddress,
    DWORD *pdwCount,
    FILETIME *pFileTime,
    LPCTSTR pszShellExecuteCommand,
    int cchShellExecuteCommand
);
static HANDLE hShell32Dll = (HANDLE)0;
static SHGetUnreadMailCountW_t SHGetUnreadMailCountW = NULL;
#endif

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

/**************************************************************************
** LH_QtPlugin_Mailcount
**************************************************************************/

LH_QtPlugin_Mailcount thePlugin;

const lh_blob *LH_QtPlugin_Mailcount::lh_logo()
{
    static char logo_blob_data[ sizeof(lh_blob) + 3405 ]; // 3405 = filesize of 'envelope48.png'
    static lh_blob *logo_blob = NULL;

    if( logo_blob == NULL )
    {
        logo_blob = (lh_blob *) (void*) & logo_blob_data;
        QFile file(":/mailcount/images/envelope48.png");
        file.open(QIODevice::ReadOnly);
        QByteArray array = file.readAll();
        Q_ASSERT( array.size() + sizeof(lh_blob) == sizeof(logo_blob_data) );
        memset( logo_blob_data, 0, sizeof(logo_blob_data) );
        logo_blob->len = array.size();
        logo_blob->sign = 0xDEADBEEF;
        memcpy( & logo_blob->data, array.constData(), array.size() );
    }

    return logo_blob;
}

const char *LH_QtPlugin_Mailcount::lh_load()
{
#ifdef Q_WS_WIN
    hShell32Dll = LoadLibraryW( L"SHELL32.DLL" );
    if( hShell32Dll != NULL )
        SHGetUnreadMailCountW = (SHGetUnreadMailCountW_t) GetProcAddress( (HMODULE) hShell32Dll, "SHGetUnreadMailCountW" );
#endif
    return NULL;
}

void LH_QtPlugin_Mailcount::lh_unload()
{
#ifdef Q_WS_WIN
    if( hShell32Dll )
    {
            FreeLibrary( (HMODULE) hShell32Dll );
            hShell32Dll = NULL;
            SHGetUnreadMailCountW = 0;
    }
#endif
    return;
}

/**************************************************************************
** LH_Mailcount
**************************************************************************/

LH_PLUGIN_CLASS(LH_Mailcount)

lh_class *LH_Mailcount::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "System/Mail",
        "SystemMailIcon",
        "Unread mail (Icon)",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classinfo;
}

LH_Mailcount::LH_Mailcount( const char *name ) : LH_QtInstance(name)
{
    count_ = 0;
    envelope_count_ = -1;

    email_addr_ = new LH_Qt_QString(this,tr("Only check address"),QString(),LH_FLAG_AUTORENDER);
    email_days_ = new LH_Qt_int(this,tr("Days back to check"),7,LH_FLAG_AUTORENDER);
    mail_image_ = new LH_Qt_QFileInfo(this,tr("Mail envelope image"),QFileInfo(),LH_FLAG_AUTORENDER);
    connect( mail_image_, SIGNAL(changed()), this, SLOT(makeEnvelope()) );
    check_interval_ = new LH_Qt_int(this,tr("Check interval (seconds)"),2,LH_FLAG_AUTORENDER);

    flashing_ = new LH_Qt_bool(this,tr("Flash when there is mail"),true);
    connect( flashing_, SIGNAL(changed()), this, SLOT(requestPolling()) );
    smoothflash_ = new LH_Qt_bool(this,tr("Use smooth flashes"),false);
    connect( smoothflash_, SIGNAL(changed()), this, SLOT(requestPolling()) );

    flash_on_ = true;
}

void LH_Mailcount::input(lh_setup_item *item, int flags, int value)
{
    Q_UNUSED(item);
    Q_UNUSED(flags);
    Q_UNUSED(value);
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
    Q_UNUSED(p);

    if( n & LH_NOTE_SECOND )
    {
        int delta = last_check_.secsTo( QTime::currentTime() );
        if( envelope_count_ == -1 ||
            !last_check_.isValid() ||
            delta < 0 ||
            delta > check_interval_->value() )
        {
            last_check_ = QTime::currentTime();
            count_ = getUnreadMailcount();
        }

        flash_on_ = !flash_on_;

        if( count_ != envelope_count_ || (flashing_->value() && envelope_count_) )
            requestRender();
    }

    return LH_NOTE_SECOND;
}

void LH_Mailcount::prerender()
{
}

int LH_Mailcount::width(int h)
{
    if( h < 0 ) return 48;
    if( !h ) return 0;
    if( h > 48 ) return 16;;
    return count_ ? (h/2) * (count_+1) : h;
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

    if( count_ < 1 )
    {
        envelope_count_ = count_;
        return;
    }

    if( ((qreal)image_->width()/(qreal)stamp.width()) >=
        ((qreal)image_->height()/(qreal)stamp.height()) )
    {
        // left to right
        span = (image_->width() / 2) - (stamp.width() / 2);
        dx = span / count_;
        if( dx > stamp.width() / 2 ) dx = stamp.width() / 2;
        if( dx < 1 ) dx = 1;
        x = span - ((dx*(count_-1))/2);
        if( x < 0 ) x = 0;
        y = dy = 0;
    }
    else
    {
        // top to bottom
        span = (image_->height() / 2) - (stamp.height() / 2);
        dy = span / count_;
        if( dy > stamp.height() / 2 ) dy = stamp.height() / 2;
        if( dy < 1 ) dy = 1;
        y = span - ((dy*(count_-1))/2);
        if( y < 0 ) y = 0;
        x = dx = 0;
    }

    QPainter painter;
    if( painter.begin(&envelope_) )
    {
        for( int i=0; i<count_; i++ )
        {
            painter.drawImage( x, y, stamp );
            x += dx;
            y += dy;
        }
        envelope_count_ = count_;
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

    if( count_ != envelope_count_ )
    {
        makeEnvelope();
    }

    image_->fill( qRgba(0,0,0,0) );
    if( !count_ ) return image_;

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

int LH_Mailcount::getUnreadMailcount()
{
    int total = 0;

#ifdef Q_WS_WIN
    if( SHGetUnreadMailCountW )
    {
        HRESULT retv;
        SYSTEMTIME st;
        FILETIME ft;
        ULARGE_INTEGER mailtime;
        DWORD dwMail = 0;

        /* Get unread mail count */
        GetLocalTime( &st );
        SystemTimeToFileTime( &st, &ft );
        Q_ASSERT( sizeof(mailtime) == sizeof(ft) );
        memcpy( &mailtime, &ft, sizeof(mailtime) );
        mailtime.QuadPart -= (ULONGLONG)email_days_->value()*24*60*60*10000000; /* subtract wanted number of days */
        memcpy( &ft, &mailtime, sizeof(mailtime) );
        if( !email_addr_->value().isEmpty() )
        {
            retv = SHGetUnreadMailCountW( NULL, (LPCTSTR)(void*)email_addr_->value().utf16(), &dwMail, &ft, NULL, 0 );
        }
        else
            retv = SHGetUnreadMailCountW( NULL, NULL, &dwMail, &ft, NULL, 0 );
        if( retv != S_OK ) dwMail = 0;

        total += dwMail;
    }
#endif

    return total;
}
