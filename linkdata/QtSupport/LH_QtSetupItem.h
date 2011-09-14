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
#include <QVariant>
#include <QColor>
#include <QFont>
#include <QFileInfo>
#include <QImage>
#include <QStringList>

#include "LH_QtVariant.h"
#include "LH_QtObject.h"

/* compatibility with A18 */
/* from A19, having no flags disables an item completely, in A18 some where negating flags */
/* using lh_meta_default in A19 has the same effect as setting flags to zero in A18 */
/* LH_FLAG_MIN, LH_FLAG_MAX and LH_FLAG_MINMAX are no longer applicable, use hasMinimum() if needed */
#define LH_FLAG_READONLY    (lh_meta_flag_unused<<0)
#define LH_FLAG_HIDDEN      (lh_meta_flag_unused<<1)
#define LH_FLAG_FOCUS       (lh_meta_flag_unused<<2)
#define LH_FLAG_AUTORENDER  (lh_meta_flag_unused<<3)
#define LH_FLAG_FIRST       (lh_meta_flag_unused<<4)
#define LH_FLAG_LAST        (lh_meta_flag_unused<<5)
#define LH_FLAG_NOSAVE      (lh_meta_flag_unused<<6)
#define LH_FLAG_INDENTTITLE (lh_meta_flag_unused<<7)
#define LH_FLAG_NOSOURCE    (lh_meta_flag_unused<<8)
#define LH_FLAG_NOSINK      (lh_meta_flag_unused<<9)
#define LH_FLAG_HIDETITLE   (lh_meta_flag_unused<<10)
#define LH_FLAG_HIDEVALUE   (lh_meta_flag_unused<<11)
#define LH_FLAG_MIN         (lh_meta_flag_unused<<12)
#define LH_FLAG_MAX         (lh_meta_flag_unused<<13)
#define LH_FLAG_MINMAX      (LH_FLAG_MIN|LH_FLAG_MAX)
#define LH_FLAG_BLANKTITLE  (LH_FLAG_HIDETITLE|LH_FLAG_INDENTTITLE)

class LH_QtSetupItem : public LH_QtObject
{
    Q_OBJECT

    lh_setup_item item_;
    QByteArray help_array_;
    QByteArray link_path_array_;
    QByteArray link_filter_array_;

    void emitSpecific();
    void buffer_resize( lh_buffer *buf, int needed );

protected:
    LH_QtVariant value_;
    LH_QtVariant min_;
    LH_QtVariant max_;
    LH_QtVariant other_;

public:
    LH_QtSetupItem( LH_QtObject *parent, const char *ident, lh_meta_type type = lh_type_none, int flags = lh_meta_default );
    ~LH_QtSetupItem();

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>(LH_QtObject::parent()); }

    const lh_setup_item& item() const { return item_; }
    const lh_setup_meta& meta() const { return item_.meta; }
    const lh_setup_link& link() const { return item_.link; }

    int notify( int note, void *param );

    void value_changed();
    void param_changed();
    void link_changed();

    void refreshMeta() { callback( lh_cb_setup_refresh_meta, 0 ); }
    void refreshLink() { callback( lh_cb_setup_refresh_link, 0 ); }
    void refreshValue() { callback( lh_cb_setup_refresh_value, 0 ); }
    void refreshParam() { callback( lh_cb_setup_refresh_param, 0 ); }

    // metadata
    lh_meta_type type() const { return meta().type; }
    const char *typeName() const { return lh_meta_type_to_name( type() ); }
    int flags() const { return meta().flags; }
    int order() const { return meta().order; }
    const char *help() const { return help_array_.constData(); }
    bool hasFlag( int f ) const { return flags() & f; }
    void setFlag( int f, bool state ); // set individual flag(s) on or off
    // setFlags() is a slot
    // setOrder() is a slot
    // setHelp() is a slot

    void setType( lh_meta_type );
    void setType( const char *name ) { setType( lh_name_to_meta_type(name) ); }

    bool hasHelp() const { return !help_array_.isEmpty(); }
    bool hasMinimum() const { return !min_.isNull(); }
    bool hasMaximum() const { return !max_.isNull(); }
    bool hasMinMax() const { return hasMinimum() && hasMinimum(); }

    bool isVisible() const { return flags()&lh_meta_show; }
    bool isHidden() const { return !isVisible(); }
    bool isWriteable() const { return flags()&lh_meta_enabled; }
    bool isReadonly() const { return !isWriteable(); }
    bool isSaving() const { return flags()&lh_meta_save; }
    bool isAutorendering() const { return flags()&lh_meta_autorender; }
    bool isFocusable() const { return flags()&lh_meta_focus; }

    const LH_QtVariant& value() const { return value_; }
    const LH_QtVariant& minimum() const { return min_; }
    const LH_QtVariant& maximum() const { return max_; }
    const LH_QtVariant& other() const { return other_; }

    void setMinMax( const QVariant& min, const QVariant& max )
    {
        setMinimum(min);
        setMaximum(max);
    }

    // link
    const char *linkPath() const { return link_path_array_.constData(); }
    const char *linkFilter() const { return link_filter_array_.constData(); }
    int linkFlags() const { return item_.link.flags; }
    bool isSource() const { return item_.link.path && item_.link.flags&lh_link_source; }
    bool isSink() const { return item_.link.path && !item_.link.flags&lh_link_source; }

    void setLinkFilter( const char *filter = 0 );

signals:
    // these signals are emitted when LCDHost changes data
    void valueChanged( const LH_QtSetupItem& );
    void minimumChanged( const LH_QtSetupItem& );
    void maximumChanged( const LH_QtSetupItem& );
    void otherChanged( const LH_QtSetupItem& );
    void linkChanged( const LH_QtSetupItem& );

    // these signals are sent when receiving corresponding notifications from LCDHost
    void duplicateSource();
    void input( int, int );

    // these signals are emitted when data is set programatically
    void flagsSet( int );
    void orderSet( int );
    void helpSet( const char * );
    void valueSet( const QVariant& );
    void paramSet();
    void linkSet( const char *path, int flags, const char *filter );

    // these signals are emitted on changes either from LCDHost or programatically
    void boolChanged( bool );
    void intChanged( int );
    void colorChanged( QColor );
    void doubleChanged( double );
    void stringChanged( QString );
    void fontChanged( QFont );
    void imageChanged( QImage );

public slots:
    void setFlags( int );
    void setOrder( int n );
    void setHelp( const QString& );
    void setHelp( const char * );

    void setVisible( bool b ) { setFlag( lh_meta_visible, b ); }
    void setHidden( bool b ) { setVisible(!b); }
    void setReadonly( bool b ) { setFlag( lh_meta_enabled, !b ); }
    void setWriteable( bool b ) { setReadonly(!b); }
    void setSaving( bool b ) { setFlag( lh_meta_save, b ); }

    void setValue( const QVariant& v );
    void setValue( bool b ) { setValue( qVariantFromValue(b) ); }
    void setValue( int i ) { setValue( qVariantFromValue(i) ); }
    void setValue( long long ll ) { setValue( qVariantFromValue(ll) ); }
    void setValue( double d ) { setValue( qVariantFromValue(d) ); }
    void setValue( void *p ) { setValue( qVariantFromValue(p) ); }
    void setValue( const QString& s ) { setValue( qVariantFromValue(s) ); }
    void setValue( const QByteArray& ba ) { setValue( qVariantFromValue(ba) ); }
    void setValue( const QColor& c ) { setValue( qVariantFromValue(c) ); }
    void setValue( const QImage& img ) { setValue( qVariantFromValue(img) ); }
    void setValue( const QFont& f ) { setValue( qVariantFromValue(f) ); }
    void setValue( const char *s, int n = -1 ) { setValue( QByteArray(s,n) ); }
    void setValue( const QFileInfo& fi ) { setValue( fi.filePath() ); }

    void setMinimum( const QVariant& );
    void setMaximum( const QVariant& );
    void setOther( const QVariant& );

    void setLink( const char *path = 0, int flags = 0, const char *filter = 0 );

};

#endif // LH_QTSETUPITEM_H
