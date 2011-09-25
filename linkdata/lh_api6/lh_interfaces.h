#ifndef LH_INTERFACES_H
#define LH_INTERFACES_H

#include <QtPlugin>
#include "lh_global.h"

class QString;
class QImage;

namespace lh_api6
{

class lcdhost_interface
{
public:
    virtual ~lcdhost_interface() {}
    virtual void lh_new( QObject * obj, QObject * parent ) const = 0;
    virtual void lh_delete( QObject * obj ) const = 0;
};

class object_interface
{
public:
    virtual ~object_interface() {}
    virtual bool lh_init( const lcdhost_interface * lh_if ) = 0;
    virtual QString lh_error() const = 0;
    virtual void lh_event_initialized() = 0;
};

class setup_interface
{
public:
    virtual ~setup_interface() {}
    virtual lh_ui_type lh_setup_ui() = 0;
    virtual void lh_event_input( const lh_input * ) = 0;
    virtual void lh_event_duplicate_source() = 0;
};

class device_interface
{
public:
    virtual ~device_interface() {}
    virtual bool lh_device_open() = 0;
    virtual bool lh_device_close() = 0;
};

class output_interface
{
public:
    virtual ~output_interface() {}
    virtual bool lh_output_render( const QImage & ) = 0;
};

class input_interface
{
public:
    virtual ~input_interface() {}
    virtual QString lh_input_name( int code ) = 0;
    virtual bool lh_input_emulate( int code, int value ) = 0;
};

class item_interface
{
public:
    virtual ~item_interface() {}
    virtual void lh_item_prerender( int bitdepth ) = 0;
    virtual int lh_item_width( int height ) = 0;
    virtual int lh_item_height( int width ) = 0;
    virtual bool lh_item_render( QImage&, int width, int height, int bitdepth ) = 0;
};

class widget_interface
{
public:
    virtual ~widget_interface() {}
    virtual QString lh_widget_path() = 0;
    virtual int lh_widget_width() = 0;
    virtual int lh_widget_height() = 0;
    virtual QObject * lh_widget_create() = 0;
    virtual void lh_widget_destroy( QObject * ) = 0;
};

}

Q_DECLARE_INTERFACE( lh_api6::object_interface, "se.linkdata.lh.object_interface/6.0" )
Q_DECLARE_INTERFACE( lh_api6::setup_interface, "se.linkdata.lh.setup_interface/6.0" )
Q_DECLARE_INTERFACE( lh_api6::device_interface, "se.linkdata.lh.device_interface/6.0" )
Q_DECLARE_INTERFACE( lh_api6::output_interface, "se.linkdata.lh.output_interface/6.0" )
Q_DECLARE_INTERFACE( lh_api6::input_interface, "se.linkdata.lh.input_interface/6.0" )
Q_DECLARE_INTERFACE( lh_api6::item_interface, "se.linkdata.lh.item_interface/6.0" )
Q_DECLARE_INTERFACE( lh_api6::widget_interface, "se.linkdata.lh.widget_interface/6.0" )

#endif // LH_INTERFACES_H
