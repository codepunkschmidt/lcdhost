/**
  \file     object.h
  \brief    Implements the LCDHost Qt C++ API.
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

#ifndef LH_API6_LH_OBJECT_H
#define LH_API6_LH_OBJECT_H

#include <QObject>
#include <QString>
#include <QVariant>
#include "lh_id.h"
#include "lh_event.h"

namespace lh {
namespace api6 {

class object : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString error READ error WRITE setError STORED false )

    QString error_;
    id_ptr lh_id_;

protected:
    /**
      The only object that may be constructed without
      a parent is the plugin object, and that's created
      by LCDHost when loading the plugin.
      */
    object() : QObject() {}

public:
    /**
      Base constructor for LCDHost plugin objects. Note that
      you must have a valid (although not nessecarily
      initalized) parent object.
      */
    object( object & parent, const char * objectname = 0 ) :
        QObject( & parent )
    {
        if( objectname ) setObjectName( objectname );
        return;
    }

    /**
      lh::object descendant destructors must be able to
      clean up in an orderly fashion regardless of whether
      \c init() has been called or not.
      */
    virtual ~object()
    {
        lh_destroy();
        return;
    }

    /**
      Override to return lh_object *
      */
    object *parent() const
    {
        return static_cast<object*>( QObject::parent() );
    }

    /**
      Overridden to ensure the object name conforms
      to LCDHost conventions.
      */
    void setObjectName( const QString & name )
    {
        QObject::setObjectName( ident( name, this ) );
        return;
    }

    /**
      Returns the LCDHost ID object.
      */
    const id_ptr & id() const
    {
        return lh_id_;
    }

    /**
      Called by LCDHost to provide the LCDHost ID.
      This is called once the corresponding LCDHost object
      has been created. Any pre-existing children will have
      their lh_create() function called. Then the init()
      function will be called. Use setError() to report
      an error condition. If init() return false, lh_destroy()
      will be called to disassociate from LCDHost.

      \param id The LCDHost ID.
      \return Result from init()
      */
     bool lh_init( const id_ptr & id );

    /**
      Request that LCDHost create it's corresponding
      object and call lh_init() on this object. If
      the current lh_object is already initialized,
      there is no parent object, or if the parent
      is not initialized, the call has no effect.
      */
    void lh_create()
    {
        if( !lh_id_ && parent() && parent()->lh_id_ )
            parent()->lh_id_->lh_new( *this );
        return;
    }

    /**
      De-register this object with LCDHost. This requests
      LCDHost to destroy it's corresponding object. If the
      object is not initialized, the call has no effect.
      */
    void lh_destroy()
    {
        if( lh_id_ )
        {
            lh_id_->lh_delete();
            lh_id_.reset();
        }
        return;
    }

    /**
      Call the base class implementation of init()
      and only proceed if it returns \c true. Return
      true if you successfully initialized. On error,
      set the error message using \c setError(),
      and return \c false.
      */
    virtual bool init()
    {
        return true;
    }

    /**
      Return the object's error string, or an empty
      string if there is no error condition.
      */
    const QString & error() const
    {
        return error_;
    }

    void setError( const QString & err )
    {
        error_ = err;
        return;
    }

    void lh_request_polling() const
    {
        if( lh_id_ ) lh_id_->lh_request_polling();
        return;
    }

    void lh_request_render() const
    {
        if( lh_id_ ) lh_id_->lh_request_render();
        return;
    }

    void lh_request_reload( const QString & reason ) const
    {
        if( lh_id_ ) lh_id_->lh_request_reload( reason );
        return;
    }

    void customEvent( QEvent * );

    /**
      Sanitizes a string to conform to LCDHost identity string rules.
      If the \p obj parameter is given, it will also auto-generate
      an identity string for the given object if needed.
      \param str The string to sanitize.
      \param obj Optional object to sanitize for.
      \return A reasonable LCDHost identity string.
      */
    static QString ident( const QString & str, const QObject *obj = 0 );

signals:
    void initialized();
    void titleChanged( const QString & title );
};

namespace event {

class initchild : public QEvent
{
    id_ptr id_;
    QString childname_;
public:
    static const Type type = (Type) type_initchild;
    initchild( const id_ptr & id, const QString & childname ) :
        QEvent( type ),
        id_( id ),
        childname_( childname )
    {}
    const id_ptr & id() const { return id_; }
    const QString & childName() const { return childname_; }
};

class setproperty : public QEvent
{
    const char * name_;
    QVariant value_;
public:
    static const Type type = (Type) type_setproperty;
    setproperty( const char * name, const QVariant & value ) :
        QEvent( type ),
        name_( name ),
        value_( value )
    {}
    const char * name() const { return name_; }
    const QVariant & value() const { return value_; }
};

} // namespace event

} // namespace api6
} // namespace lh

Q_DECLARE_INTERFACE( lh::api6::object, "se.linkdata.lh_object/6.0" )

#endif // LH_API6_LH_OBJECT_H
