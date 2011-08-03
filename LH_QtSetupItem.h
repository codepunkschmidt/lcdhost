/**
  \file     LH_QtSetupItem.h
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

#ifndef LH_QTSETUPITEM_H
#define LH_QTSETUPITEM_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QFileInfo>

#include "LH_QtObject.h"

class LH_QtSetupItem : public LH_QtObject
{
    Q_OBJECT

    QByteArray ident_array_;
    QByteArray title_array_;
    QByteArray link_array_;
    QByteArray help_array_;
    QByteArray list_array_;

protected:
    QString str_; // string helper
    QByteArray data_array_;
    lh_setup_item item_;

    void setArray( const QByteArray& a )
    {
        data_array_ = a;
        item_.data.b.p = data_array_.data();
        item_.data.b.n = data_array_.capacity();
    }

    void setString( const QString& s )
    {
        str_ = s;
        setArray( str_.toUtf8() );
    }

    void getString()
    {
        str_ = QString::fromUtf8( (const char*) item_.data.b.p );
    }

public:
    LH_QtSetupItem( LH_QtObject *parent, const char *ident, lh_setup_type type, int flags );
    ~LH_QtSetupItem();

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>(LH_QtObject::parent()); }

    int notify( int note, void *param );
    void setup_resize( size_t needed );
    virtual void setup_change(); // virtual to allow subclasses to update derived object caches

    void refreshMeta() { callback( lh_cb_setup_refresh_meta, 0 ); }
    void refreshData() { callback( lh_cb_setup_refresh_data, 0 ); }

    // metadata
    void setIdent(const char * s);
    const char *ident() const { return ident_array_.constData(); }
    void setTitle(const QString& s);
    const char *title() const { return title_array_.constData(); }
    void setLink(const char * s);
    const char *link() const { return link_array_.constData(); }
    void setHelp(const QString& s);
    const char *help() const { return help_array_.constData(); }
    int flags() const { return item_.flags; }
    bool hasFlag( int f ) const { return (item_.flags & f) ? true : false; }
    void setFlags( int f ) { if( item_.flags != f ) { item_.flags = f; refreshMeta(); } }
    void setFlag( int f, bool state ); // set individual flag(s) on or off

    void setMin( double );
    void setMax( double );
    void setMinMax( double, double );

    void setMin( qint64 );
    void setMax( qint64 );
    void setMinMax( qint64, qint64 );

    void setMin( int n ) { setMin( (qint64)n ); }
    void setMax( int n ) { setMax( (qint64)n ); }
    void setMinMax( int a, int b ) { setMinMax( (qint64)a, (qint64)b ); }

    void setList( const QByteArray& list );
    QByteArray& list() { return list_array_; } // call refreshList() if you modify it
    void refreshList();

    // data
    lh_setup_item *item() { return &item_; }
    lh_setup_type type() const { return item_.type; }
    int order() const { return item_.order; }
    void setOrder( int n );

signals:
    void change( bool );
    void change( qint64 );
    void change( double );
    void change( QString );
    void change( QColor );
    void change( QFont );
    void change( QFileInfo );
    void changed(); // changed from LCDHost
    void set(); // set programatically using setValue()
    void input( int, int );
    void duplicateSource();

public slots:
    void setVisible( bool b ) { setFlag( LH_FLAG_HIDDEN, !b ); }
    void setHidden( bool b ) { setFlag( LH_FLAG_HIDDEN, b ); }
    void setReadonly( bool b ) { setFlag( LH_FLAG_READONLY, b ); }
    void setWriteable( bool b ) { setFlag( LH_FLAG_READONLY, !b ); }
    void setSaving( bool b ) { setFlag( LH_FLAG_NOSAVE, !b ); }

    void setValue( bool );
    void setValue( int );
    void setValue( const QColor& );
    void setValue( qlonglong );
    void setValue( double );
    void setValue( void * );
    void setValue( const QString& );
    void setValue( const QByteArray& );
    void setValue( const char *, int len = -1 );
};

#endif // LH_QTSETUPITEM_H
