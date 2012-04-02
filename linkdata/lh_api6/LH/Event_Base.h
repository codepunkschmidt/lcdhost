/**
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2012 Johan Lindh

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

#ifndef LH_EVENT_BASE_H
#define LH_EVENT_BASE_H

#include <QEvent>
#include <QByteArray>

namespace LH {
namespace Event {

/**
  Event types registered with QEvent::registerEventType()
  won't match up between shared libraries. This function
  maintains a name <-> value map for event types.
  */
QEvent::Type typeHash( const char ** p_classname = 0, QEvent::Type lookup = QEvent::None );

/**
  Register a LCDHost event by it's class name. Used by the
  LH_EVENT() macro to create the event class' static type()
  function.
*/
static inline QEvent::Type typeRegister( const char * classname )
{
    return typeHash( & classname );
}

/**
  Given a QEvent::Type previously returned from typeRegister(),
  returns a QByteArray with the event class name. Returns
  a null QByteArray if the type hasn't been registered.
*/
static inline const QByteArray typeName( QEvent::Type lookup )
{
    const char * classname = 0;
    typeHash( & classname, lookup );
    return QByteArray( classname );
}

#define LH_EVENT( classname ) \
    static QEvent::Type type() { \
        static QEvent::Type type_ = LH::Event::typeRegister( #classname ); \
        return type_; \
    }

class Base : public QEvent
{
public:
    explicit Base( QEvent::Type t ) : QEvent( t ) {}
};

/**
  Named events too trivial to deserve their own header.
  */

class Destroy : public Base
{
public:
    LH_EVENT(Destroy)
    Destroy() : Base( type() ) {}
};

class Ping : public Base
{
public:
    LH_EVENT(Ping)
    Ping() : Base( type() ) {}
};

class InitSuccess : public Base
{
public:
    LH_EVENT(InitSuccess)
    InitSuccess() : Base( type() ) {}
};

class InitFailure : public Base
{
public:
    LH_EVENT(InitFailure)
    InitFailure() : Base( type() ) {}
};

} // namespace Event
} // namespace LH

#endif // LH_EVENT_BASE_H
