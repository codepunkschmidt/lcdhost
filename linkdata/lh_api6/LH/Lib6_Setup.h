#ifndef LH_LIB6_SETUP_H
#define LH_LIB6_SETUP_H

#include <LH/Lib6.h>
#include <LH/Lib6_Linkable.h>
#include <QStringList>

namespace LH {
namespace Lib6 {

/**
  Provides a linkable, saved setup item with optional UI.
  */
class Setup : public Linkable
{
    Q_OBJECT
    Q_PROPERTY( ui_type ui READ ui WRITE setUi STORED false )
    Q_PROPERTY( bool isStored READ isStored WRITE setStored STORED false )
    Q_PROPERTY( bool isEnabled READ isEnabled WRITE setEnabled STORED false )
    Q_PROPERTY( bool isVisible READ isVisible WRITE setVisible STORED false )
    Q_PROPERTY( int order READ order WRITE setOrder STORED isStored )
    Q_PROPERTY( QString title READ title WRITE setTitle STORED false )
    Q_PROPERTY( QString help READ help WRITE setHelp STORED false )
    Q_PROPERTY( QStringList list READ list WRITE setList STORED false )

    Q_INTERFACES( LH::Lib6::Object LH::Lib6::Linkable )

    int meta_;
    int order_;
    QString title_;
    QString help_;
    QStringList list_;

public:

    /**
      User interface flags for setup items.
      */
    enum ui_flag
    {
        is_stored = (1<<0), /**< this setup item should be saved if modified */
        is_visible = (1<<1), /**< show the setup item in the UI */
        is_enabled = (1<<2), /**< enable the setup item in the UI */
        is_source = (1<<3), /**< allow the UI to use this setup item as a data source */
        is_sink = (1<<4), /**< allow the UI to use this setup item as a data sink */
        is_focus = (1<<5), /**< this setup item actively wants focus */
        is_first = (1<<6), /**< same as setOrder(-1) */
        is_last = (1<<7), /**< same as setOrder(1) */
        ui_flag_mask = (is_last<<1)-1
    };
    Q_FLAGS( ui_flag )

    /**
      User interface type for setup items.
      */
    enum ui_type
    {
        ui_none = (1<<16),
        ui_checkbox, /**< checkbox */
        ui_color, /**< line editor with color picker */
        ui_font, /**< line editor with font picker */
        ui_image, /**< decode PNG data and show the image */
        ui_spinbox, /**< line editor with spinbox (optional min/max) */
        ui_slider, /**< slider (required min/max) */
        ui_progress, /**< progress bar (optional min/max) */
        ui_string, /**< line editor */
        ui_text, /**< multiline text editor */
        ui_filename, /**< line editor and file selection dialog (optional other: string: file selection mask) */
        ui_button, /**< a button (required other: string: button text) */
        ui_htmllink, /**< clicked link in title text text */
        ui_dropdownbox, /**< dropdown box (required other: stringlist: selectable values) */
        ui_listbox, /**< listbox (required other: stringlist: selectable values) */
        ui_combobox, /**< combobox (optional other: stringlist: selectable values) */
        ui_input_value, /**< readonly text and input selection dialog for button or axis */
        ui_input_state, /**< readonly text and input selection dialog for button state or half-axis */
        ui_type_mask = ~(ui_none-1)
    };
    Q_ENUMS( ui_type )

    static const int ui_default = (is_stored|is_visible|is_enabled|is_source|is_sink);
    static const char * ui_name( ui_type t );
    static ui_type ui_name( const char * name );

    explicit Setup(
            Object & parent,
            const char * ident,
            const int meta,
            const QVariant & val = QVariant(),
            const QVariant & min = QVariant(),
            const QVariant & max = QVariant()
            ) :
        Linkable( parent, ident, QString(), false, val, min, max ),
        meta_( meta ),
        order_( 0 ),
        title_( ident )
    {
        if( meta_ & is_first ) -- order_;
        if( meta_ & is_last ) ++ order_;
        lh_create();
    }

    int meta() const { return meta_; }
    ui_type ui() const { return (ui_type) (meta_ & ui_type_mask); }
    int flags() const { return meta_ & ui_flag_mask; }
    bool flag( const int f ) const { return (flags() & f) ? true : false; }
    bool isStored() const { return flag( is_stored ); }
    bool isEnabled() const { return flag( is_enabled ); }
    bool isVisible() const { return flag( is_visible ); }
    int order() const { return order_; }
    const QString & title() const { return title_; }
    const QString & help() const { return help_; }
    const QStringList & list() const { return list_; }

    void setUi( ui_type ui ) { setMeta( (meta_ & ~ui_type_mask) | ui ); }
    void setFlag( const int f, const bool b ) { setMeta( (meta_&~f) | (b?f:0) ); }
    void setStored( const bool b ) { setFlag( is_stored, b ); }
    void setEnabled( const bool b ) { setFlag( is_enabled, b ); }
    void setVisible( const bool b ) { setFlag( is_visible, b ); }

signals:
    void metaChanged( const int );

public slots:
    void setMeta( int meta );
    void setOrder( int order );
    void setTitle( const QString & title );
    void setHelp( const QString & help );
    void setList( const QStringList & sl );
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::Setup, "se.linkdata.LibSetup/6.0" )

#endif // LH_LIB6_SETUP_H
