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

#include "LH_QtObject.h"

/**
  Helper class to manage setup items. Subclass this, mapping the LCDHost setup types to Qt classes.
  */
class LH_API5PLUGIN_EXPORT LH_QtSetupItem : public QObject
{
    Q_OBJECT

    int order_;
    QByteArray name_array_;
    QByteArray help_;
    QByteArray publish_path_;

protected:
    lh_setup_item item_;

public:
    LH_QtSetupItem( LH_QtObject *parent, QString name, lh_setup_type type, int flags );

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>(QObject::parent()); }

    virtual void setup_resize( size_t ) {}
    virtual void setup_change();
    virtual void setup_input( int flags, int value );

    void refresh();

    void setFlags( int f ) { if( item_.flags != f ) { item_.flags = f; refresh(); } }
    int flags() const { return item_.flags; }
    void setFlag( int f, bool state ); // set individual flag(s) on or off
    bool hasFlag( int f ) const { return (item_.flags & f) ? true : false; }

    void setName(QString s);
    QString name() const { return objectName(); }
    void setHelp(QString s);
    QString help();

    void setLink(QString s);
    QString link() { return subscribePath(); }

    void setPublishPath(const QString &s);
    QString publishPath() const;

    void setSubscribePath(const QString &s);
    QString subscribePath() const;

    void setMimeType( const char * s );
    const char *mimeType() const { return item_.link.mime; }

    lh_setup_item *item() { return &item_; }

    lh_setup_type type() { return item_.type; }

    int order() const { return order_; }
    void setOrder( int n );

    bool isVisible() const { return !isHidden(); }
    bool isHidden() const { return flags()&LH_FLAG_HIDDEN; }

signals:
    void changed();
    void change( bool );
    void change( int );
    void change( float );
    void change( QString );
    void input( QString, int, int );
    void set();

public slots:
    void setVisible( bool b ) { setFlag( LH_FLAG_HIDDEN, !b ); }
    void setHidden( bool b ) { setFlag( LH_FLAG_HIDDEN, b ); }
    void setReadonly( bool b ) { setFlag( LH_FLAG_READONLY, b ); }
    void setWriteable( bool b ) { setFlag( LH_FLAG_READONLY, !b ); }
    void setSaving( bool b ) { setFlag( LH_FLAG_NOSAVE_DATA|LH_FLAG_NOSAVE_LINK, !b ); }
    void setSavingData( bool b ) { setFlag( LH_FLAG_NOSAVE_DATA, !b ); }
    void setSavingLink( bool b ) { setFlag( LH_FLAG_NOSAVE_LINK, !b ); }

    virtual void setValue( bool ) { emit set(); }
    virtual void setValue( int ) { emit set(); }
    virtual void setValue( float ) { emit set(); }
    virtual void setValue( QString ) { emit set(); }
};

#endif // LH_QTSETUPITEM_H
