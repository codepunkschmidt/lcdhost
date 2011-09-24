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

#include "lh_qvariant.h"
#include "LH_QtObject.h"

#define LH_API5_COMPAT

#ifdef LH_API5_COMPAT

typedef enum lh_setup_type_t
{
    // API5, maintain exact order and values from the 'API5' branch lh_plugin.h
    lh_type_none,
    lh_type_integer,
    lh_type_integer_boolean, /* checkbox */
    lh_type_integer_color, /* 32-bit AARRGGBB */
    lh_type_integer_slider,
    lh_type_integer_progress,
    lh_type_integer_list, /* using dropdown box, have user select one of the param.list strings */
    lh_type_fraction,
    lh_type_string, /* all strings are null-terminated, utf-8 encoded */
    lh_type_string_script, /* inline script */
    lh_type_string_filename, /* present the user with an file selection dialog */
    lh_type_string_font, /* simple font selection, see QFont::toString() for string format */
    lh_type_string_inputstate, /* ask user for a specific button state or half axis movement */
    lh_type_string_inputvalue, /* ask user for a button or full axis movement */
    lh_type_image_png, /* allows the display of a PNG image in the setup pane (data.s and param.size used) */
    lh_type_image_qimage, /* allows the display of a QImage in the setup pane static_cast<QImage*>(data.s) */
    lh_type_integer_listbox, /* using listbox, have user select one of the param.list strings */
    lh_type_string_button, /* a clickable button */
    lh_type_string_htmlhint, /* static HTML text, for best effect start the name with '~' to hide it */

    // caught between API5 and 6
    lh_type_string_list, /* using dropdown box, have user select one of the param.list strings */
    lh_type_string_listbox, /* using listbox, have user select one of the param.list strings */
    lh_type_string_combobox, /* using combobox, allow user to select or type a string value */
    lh_type_pointer = 0x0800, /* not saved - store a pointer */
    lh_type_pointer_qimage, /* not saved - allows the display of a QImage in the setup pane, param.p */
    lh_type_array = 0x1000, /* basic fixed-size data storage */
    lh_type_array_png, /* allows the display of a PNG image */
    lh_type_array_qint64, /* no UI - store an array of qint64 in buffer */
    lh_type_array_double, /* no UI - store an array of doubles in buffer */
    lh_type_array_string, /* no UI - data is a list of NUL-delimited UTF-8 strings */

    // aliases
    lh_type_double = lh_type_fraction, /* double selection using spinbox */
    lh_type_string_html = lh_type_string_htmlhint, /* static HTML text, for best effect start the name with '~' to hide it */
    lh_type_string_htmllink = lh_type_string_htmlhint, /* receive clicked links from the title */
    lh_type_string_dropdownbox = lh_type_string_list,

    lh_type_last /* marks last used value */

} lh_setup_type;

#define LH_FLAG_READONLY    0x0001 /* Setup item is disabled in GUI */
#define LH_FLAG_HIDDEN      0x0002 /* Setup item is excluded from GUI */
#define LH_FLAG_FOCUS       0x0004 /* Setup item is focused when user double clicks instance */
#define LH_FLAG_AUTORENDER  0x0008 /* Request a render when changed */
#define LH_FLAG_FIRST       0x0010 /* Used with LH_QtSetupItem's - Same as calling setOrder(-1) */
#define LH_FLAG_LAST        0x0020 /* Used with LH_QtSetupItem's - Same as calling setOrder(1) */
#define LH_FLAG_NOSAVE      0x0040 /* Setup item won't be saved to or loaded from layout */
#define LH_FLAG_INDENTTITLE 0x0080
#define LH_FLAG_NOSOURCE    0x0100 /* Setup item must not be used as a data source */
#define LH_FLAG_NOSINK      0x0200 /* Setup item must not be used as a data sink */
#define LH_FLAG_HIDETITLE   0x0400 /* Setup item title is not shown in GUI (all space to value) */
#define LH_FLAG_HIDEVALUE   0x0800 /* Setup item value is not shown in GUI (all space to title) */
#define LH_FLAG_MIN         0x1000 /* Limit UI numeric input to the min value given in params */
#define LH_FLAG_MAX         0x2000 /* Limit UI numeric input to the max value given in params */

#define LH_FLAG_MINMAX      (LH_FLAG_MIN|LH_FLAG_MAX) /* Limit UI numeric input to the min and max value given in params */
#define LH_FLAG_BLANKTITLE  (LH_FLAG_INDENTTITLE|LH_FLAG_HIDETITLE) /* Setup item title is not shown in GUI (blank space is shown) */
#endif

class LH_QtSetupItem : public LH_QtObject
{
    Q_OBJECT

    lh_setup_item item_;
    QByteArray help_array_;
    QList<LH_QVariant> properties_; // keep 'value' property always there and at first!

    void emitSpecific();
    void buffer_resize( lh_buffer *buf, int needed );
    void setIdent( const char *ident );
    void create( const char *ident, int metainfo );

protected:
#ifdef LH_API5_COMPAT
    int api5flags_;
    lh_setup_type api5type_;
    void api5create( const char *ident, int flags, lh_setup_type type );

    LH_QtSetupItem( LH_QtObject *parent, const char *ident, const QVariant& value, int flags, lh_setup_type type ) :
        LH_QtObject( &item_.obj, ident, parent )
    {
        properties_.append(LH_QVariant(value,lh_role_value));
        api5create( ident, flags, type );
    }
#endif

public:
    LH_QtSetupItem( LH_QtObject *parent, const char *ident, const LH_QVariant& data1, int metainfo = lh_meta_default ) :
        LH_QtObject( &item_.obj, ident, parent )
    {
        properties_.append(LH_QVariant(lh_role_value));
        setProperty(data1);
        create( ident, metainfo );
    }

    LH_QtSetupItem( LH_QtObject *parent, const char *ident, const LH_QVariant& data1, const LH_QVariant& data2, int metainfo = lh_meta_default ) :
        LH_QtObject( &item_.obj, ident, parent )
    {
        properties_.append(LH_QVariant(lh_role_value));
        setProperty(data1);
        setProperty(data2);
        create( ident, metainfo );
    }

    LH_QtSetupItem( LH_QtObject *parent, const char *ident, const LH_QVariant& data1, const LH_QVariant& data2, const LH_QVariant& data3, int metainfo = lh_meta_default ) :
        LH_QtObject( &item_.obj, ident, parent )
    {
        properties_.append(LH_QVariant(lh_role_value));
        setProperty(data1);
        setProperty(data2);
        setProperty(data3);
        create( ident, metainfo );
    }

    LH_QtSetupItem( LH_QtObject *parent, const char *ident, const LH_QVariant& data1, const LH_QVariant& data2, const LH_QVariant& data3, const LH_QVariant& data4, int metainfo = lh_meta_default ) :
        LH_QtObject( &item_.obj, ident, parent )
    {
        properties_.append(LH_QVariant(lh_role_value));
        setProperty(data1);
        setProperty(data2);
        setProperty(data3);
        setProperty(data4);
        create( ident, metainfo );
    }

    ~LH_QtSetupItem();

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>(LH_QtObject::parent()); }

    const lh_setup_item& item() const { return item_; }

    int notify( int note, void *param );

    void update_data( const lh_meta datainfo, const void *ptr );

#ifdef LH_API5_COMPAT
    lh_setup_type type() const { return api5type_; }
    int flags() const { return api5flags_; }
    bool hasFlag( int f ) const { return flags() & f; }
    void setFlag( int f, bool state ); // set individual flag(s) on or off
    const char *help() const { return help_array_.constData(); }
    const lh_setup_meta& meta() const { return item_.meta; }
#endif

    // metadata
    int ui() const { return lh_meta_ui(item_.meta.info); }
    int info() const { return item_.meta.info; }
    int order() const { return item_.meta.order; }
    // setFlags() is a slot
    // setOrder() is a slot
    // setHelp() is a slot

    bool hasData( int role ) const;
    bool hasHelp() const { return !help_array_.isEmpty(); }
    bool hasMinimum() const { return hasData(lh_role_minimum); }
    bool hasMaximum() const { return hasData(lh_role_maximum); }
    bool hasMinMax() const { return hasMinimum() && hasMinimum(); }

    bool isVisible() const { return flags()&lh_meta_show; }
    bool isHidden() const { return !isVisible(); }
    bool isWriteable() const { return flags()&lh_meta_enabled; }
    bool isReadonly() const { return !isWriteable(); }
    bool isSaved() const;
    bool isAutorendering() const { return flags()&lh_meta_autorender; }
    bool isFocusable() const { return flags()&lh_meta_focus; }
    bool isSource() const;
    bool isSink() const;

    const LH_QVariant& data( int role ) const;
    const LH_QVariant& value() const { return properties_.first(); }
    const LH_QVariant& minimum() const { return data(lh_role_minimum); }
    const LH_QVariant& maximum() const { return data(lh_role_maximum); }
    const LH_QVariant& list() const { return data(lh_role_list); }
    const LH_QVariant& link() const { return data(lh_role_link); }

    void refreshMeta()
    {
        if( item_.cb_refresh_meta && item_.obj.cb_id )
            item_.cb_refresh_meta( item_.obj.cb_id );
    }

    void setMinMax( const QVariant& min, const QVariant& max )
    {
        setMinimum(min);
        setMaximum(max);
    }

signals:
    // these signals are emitted when LCDHost changes data associated with the role
    void valueChanged( const QVariant& );
    void minimumChanged( const QVariant& );
    void maximumChanged( const QVariant& );
    void listChanged( const QVariant& );
    void linkChanged( const QVariant& );
    void userChanged( const LH_QVariant& );

    // these signals are sent when receiving corresponding notifications from LCDHost
    void duplicateSource();
    void input( int, int );

    // these signals are emitted when data is set programatically
    void flagsSet( int );
    void orderSet( int );
    void helpSet( const char * );
    void valueSet( const QVariant& );
    void minimumSet( const QVariant& );
    void maximumSet( const QVariant& );
    void listSet( const QVariant& );
    void linkSet( const QVariant& );
    void userSet( const LH_QVariant& );

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

    void setProperty( const LH_QVariant& data );
    void setValue( const QVariant& v );
    void setMinimum( const QVariant& );
    void setMaximum( const QVariant& );
    void setList( const QVariant& );
    void setLink( const QVariant& );

#if 0
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
#endif

};

#endif // LH_QTSETUPITEM_H
