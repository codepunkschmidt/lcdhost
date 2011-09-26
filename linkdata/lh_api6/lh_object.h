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

#ifndef LH_OBJECT_H
#define LH_OBJECT_H

#include <QObject>
#include <QExplicitlySharedDataPointer>
#include "lh_id.h"
#include "lh_interfaces.h"
#include "lh_qvariant.h"

namespace lh_api6
{

class object : public QObject, public object_interface
{
    Q_OBJECT

    Q_PROPERTY( QString error READ lh_error STORED false )
    Q_INTERFACES( lh_api6::object_interface )

    QString error_;
    lh_id lh_id_;

public:
    /**
      All lh::object descendants must implement default
      constructors that instantiate the object in a known
      state. This means initializing primitive data members.
      Pay particular attention to pointers. While you're not
      explicitly forbidden to perform costly operations in
      the constructor, it's good practice to do it in \c init().
      */
    object( const QString & ident = QString(), object * parent = 0 ) :
        QObject( parent )
    {
        setObjectName( ident );
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
      Overridden to ensure LCDHost C-API compliant identity strings.
      */
    void setObjectName( const QString & name )
    {
        QObject::setObjectName( object::ident( name ) );
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
      Request that LCDHost create it's corresponding
      object and call lh_init() on this object. If
      the current lh_object is already initialized,
      there is no parent object, or if the parent
      is not initialized, the call has no effect.
      */
    void lh_create()
    {
        if( !lh_id_ && parent() && parent()->lh_id_ )
            parent()->lh_id_->lh_new( this );
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
            lh_id_->lh_delete( this );
            lh_id_.reset();
        }
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

    void setError( const QString & err )
    {
        error_ = err;
    }

    // lh_api6::object_interface

    /**
      Called by LCDHost to provide the LCDHost ID.
      This is called once the corresponding LCDHost object
      has been created. Any pre-existing children will have
      their lh_create() function called. Then the init()
      function will be called. Don't reimplement this
      in a subclass unless you know what you're doing.
      */
    bool lh_init( const lh_id & id );

    /**
      Return the object's error string, or an empty
      string if there is no error condition.
      */
    QString lh_error() const
    {
        return error_;
    }

    /**
      Called by LCDHost once this object and all it's
      children have had been initialized successfully
      and all stored properties have been set.
      */
    void lh_event_initialized()
    {
        return;
    }

    // export lcdhost_interface

    void lh_request_polling() const
    {
        if( lh_id_ ) lh_id_->lh_request_polling();
    }

    void lh_request_render() const
    {
        if( lh_id_ ) lh_id_->lh_request_render();
    }

    void lh_request_reload( const QString & reason ) const
    {
        if( lh_id_ ) lh_id_->lh_request_reload( reason );
    }

    /**
      Alters an object name so that it conforms
      to the rules set for object.ident.
      */
    static QString ident( const QString& name );

signals:
    void titleChanged( const QString & title );
};

} // namespace lh_api6

typedef lh_api6::object lh_object;

#endif // LH_OBJECT_H
