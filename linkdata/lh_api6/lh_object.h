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
#include "lh_interfaces.h"
#include "lh_qvariant.h"

namespace lh_api6
{

class object : public QObject, public object_interface
{
    Q_OBJECT

    Q_PROPERTY( QString error READ error )
    Q_INTERFACES( lh_api6::object_interface )

    const lcdhost_interface * lh_if_;
    QString error_;

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
        QObject( parent ),
        lh_if_( 0 )
    {
        setObjectName( ident );
        if( parent && parent->lh_if_ )
            parent->lh_if_->lh_new( this, parent );
        return;
    }

    /**
      lh::object descendant destructors must be able to
      clean up in an orderly fashion regardless of whether
      \c init() has been called or not.
      */
    virtual ~object()
    {
        if( parent() && parent()->lh_if_ )
            parent()->lh_if_->lh_delete( this );
        lh_if_ = 0;
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
      Override to return lh::object*
      */
    object *parent() const
    {
        return static_cast<object*>( QObject::parent() );
    }

    /**
      Call the base class implementation of init()
      and only proceed if it returns \c true. Return
      true if you successfully initialized. On error,
      set the \c error_ member to reflect the problem,
      and return \c false.
      */
    virtual bool init()
    {
        return true;
    }

    const QString & error() const
    {
        return error_;
    }

    void setError( const QString & err )
    {
        error_ = err;
    }

    // object_interface

    bool lh_init( const lcdhost_interface *lh_if )
    {
        lh_if_ = lh_if;
        return init();
    }

    QString lh_error() const
    {
        return error_;
    }

    void lh_event_initialized()
    {
        return;
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
