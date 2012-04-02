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

#ifndef LH_LIB6_OBJECT_H
#define LH_LIB6_OBJECT_H

#include <LH/Lib6.h>
#include <LH/Lib6_Id.h>
#include <QObject>
#include <QString>

class QEvent;

namespace LH {
namespace Lib6 {

class Object : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString error READ error WRITE setError STORED false )

    QString error_;
    Id lh_id_;

    /**
      The only objects that may be constructed without
      a LibObject parent are services and plugins, which
      are instantiated directly by LCDHost.
      */
    friend class Service;
    Object( const QString & objectname, QObject * parent = 0 ) :
        QObject( parent )
    {
        setObjectName( objectname );
    }

protected:
    /**
      Sends all scriptable property values to LCDHost,
      returns \p init_result unless there was an error,
      in which case \c false is returned.
      */
    bool send_properties( bool init_result = true ) const;

public:
    /**
      Base constructor for LCDHost plugin objects. Note that
      you must have a valid (although not nessecarily
      initalized) parent object.
      */
    Object( Object & parent, const QString & objectname = QString() ) :
        QObject( & parent )
    {
        setObjectName( objectname );
        return;
    }

    /**
      LibObject descendant destructors must be able to
      clean up in an orderly fashion regardless of whether
      \c init() has been called or not.
      */
    virtual ~Object()
    {
        lh_destroy();
        return;
    }

    /**
      Overridden to ensure the object name conforms
      to LCDHost conventions.
      */
    void setObjectName( const QString & objectname )
    {
        QObject::setObjectName( ident( objectname, this ) );
        return;
    }

    /**
      Returns the LCDHost ID object.
      */
    const Id & id() const
    {
        return lh_id_;
    }

    /**
      Called by LCDHost to provide the LCDHost ID.
      This is called once the corresponding LCDHost object
      has been created. Any pre-existing children will have
      their lh_create() function called. Then the init()
      function will be called. Use setError() to report
      an error condition. If init() returns false, lh_destroy()
      will be called to disassociate from LCDHost.

      \param id The LCDHost ID.
      \return Result from init()
      */
     bool lh_init( const Id & id );

    /**
      Request that LCDHost create it's corresponding
      object and call lh_init() on this object. If
      the current LibObject is already initialized,
      there is no parent object, or if the parent
      is not initialized, the call has no effect.
      */
    void lh_create();

    /**
      De-register this object with LCDHost. This requests
      LCDHost to destroy it's corresponding object. If the
      object is not initialized, the call has no effect.
      */
    void lh_destroy()
    {
        if( id() )
        {
            id()->lh_destroy();
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

    /**
      Set the current error string. Returns true
      if the error string is empty, false otherwise.
      */
    bool setError( const QString & err = QString() )
    {
        error_ = err.trimmed();
        return error_.isEmpty();
    }

    void lh_request_polling() const
    {
        if( id() ) id()->lh_request_polling();
        return;
    }

    void lh_request_render() const
    {
        if( id() ) id()->lh_request_render();
        return;
    }

    void lh_request_reload( const QString & reason ) const
    {
        if( id() ) id()->lh_request_reload( reason );
        return;
    }

    void customEvent( QEvent * );

    /**
      Generate or modify a string to conform to LCDHost identity string rules.
      If the \p obj parameter is given, it will also auto-generate
      an identity string for the given object if needed.
      \param str Optional string to sanitize.
      \param obj Optional object to sanitize for.
      \return A reasonable LCDHost identity string.
      */
    static QString ident( const QString & str, const QObject * obj = 0 );

signals:
    void initialized();
    void titleChanged( const QString & title );
};

} // namespace Lib6
} // namespace LH

Q_DECLARE_INTERFACE( LH::Lib6::Object, "se.linkdata.LibObject/6.0" )

#endif // LH_LIB6_OBJECT_H
