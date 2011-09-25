#ifndef LH_INTERFACES_H
#define LH_INTERFACES_H

#include <QtPlugin>
#include "lh_global.h"

class QString;
class QImage;

class lh_object_interface
{
public:
    virtual ~lh_object_interface() {}
    virtual QString lh_object_init( const void *cb_id, lh_callback_t cb_fn ) = 0;
    virtual void lh_object_event( lh_eventcode code, void *param ) = 0;
};

class lh_setup_interface
{
public:
    virtual ~lh_setup_interface() {}
    virtual lh_ui_type lh_setup_ui() = 0;
};

class lh_device_interface
{
public:
    virtual ~lh_device_interface() {}
    virtual bool lh_device_open() = 0;
    virtual bool lh_device_close() = 0;
};

class lh_output_interface
{
public:
    virtual ~lh_output_interface() {}
    virtual bool lh_output_render( const QImage & ) = 0;
};

class lh_input_interface
{
public:
    virtual ~lh_input_interface() {}
    virtual QString lh_input_name( int code ) = 0;
    virtual bool lh_input_emulate( int code, int value ) = 0;
};

class lh_item_interface
{
public:
    virtual ~lh_item_interface() {}
    virtual void lh_item_prerender( int bitdepth ) = 0;
    virtual int lh_item_width( int height ) = 0;
    virtual int lh_item_height( int width ) = 0;
    virtual bool lh_item_render( QImage&, int width, int height, int bitdepth ) = 0;
};

class lh_widget_interface
{
public:
    virtual ~lh_widget_interface() {}
    virtual QString lh_widget_path() = 0;
    virtual int lh_widget_width() = 0;
    virtual int lh_widget_height() = 0;
    virtual QObject * lh_widget_create() = 0;
    virtual void lh_widget_destroy( QObject * ) = 0;
};

Q_DECLARE_INTERFACE( lh_object_interface, "se.linkdata.lh.object_interface/1.0" )
Q_DECLARE_INTERFACE( lh_setup_interface, "se.linkdata.lh.setup_interface/1.0" )
Q_DECLARE_INTERFACE( lh_device_interface, "se.linkdata.lh.device_interface/1.0" )
Q_DECLARE_INTERFACE( lh_output_interface, "se.linkdata.lh.output_interface/1.0" )
Q_DECLARE_INTERFACE( lh_input_interface, "se.linkdata.lh.input_interface/1.0" )
Q_DECLARE_INTERFACE( lh_item_interface, "se.linkdata.lh.item_interface/1.0" )
Q_DECLARE_INTERFACE( lh_widget_interface, "se.linkdata.lh.widget_interface/1.0" )

#endif // LH_INTERFACES_H
