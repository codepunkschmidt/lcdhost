/**
  \file     lh_qobject.cpp
  \brief    Extends the normal QObject to be aware of LCDHost properties.
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

#include <LH/Lib6_Object.h>
#include <LH/Event_InitChild.h>
#include <LH/Event_SetProperty.h>

#include <QDebug>
#include <QMetaProperty>

namespace LH {
namespace Lib6 {

bool Object::send_properties( bool init_result ) const
{
    if( !id() ) return false;

    for( int i = 0; i < metaObject()->propertyCount(); ++ i )
    {
        QMetaProperty prop( metaObject()->property(i) );
        if( prop.isScriptable() )
        {
            if( !id()->lh_set_property( prop.name(), property(prop.name()) ) )
                return false;
        }
    }

    return init_result;
}

bool Object::lh_init( const LibId & id )
{
    if( !(lh_id_ = id) )
    {
        qCritical() << metaObject()->className()
                    << objectName()
                    << "lh_init() called with null ID";
        return false;
    }

    if( !lh_id_->lh_bind( *this ) )
    {
        qCritical() << metaObject()->className()
                    << objectName()
                    << "lh_bind() failed";
        return false;
    }

    QObjectList::const_iterator it;
    for( it = children().constBegin(); it != children().constEnd(); ++ it )
    {
        if( Object *child = qobject_cast<Object *>(*it) )
            child->lh_create();
    }

    return lh_id_->lh_init_result( send_properties( init() ) );
}

void Object::lh_create()
{
    if( Object * p = qobject_cast<Object *>(parent()) )
        if( !id() && p->id() ) p->id()->lh_create_child( *this );
    return;
}

void Object::customEvent( QEvent * event )
{
    if( event->type() == LH::Event::InitSuccess::type() )
    {
        qDebug() << metaObject()->className()
                 << objectName()
                 << "fully initialized";
        emit initialized();
        return;
    }

    if( event->type() == LH::Event::Ping::type() )
    {
        id()->lh_ping_reply();
        return;
    }

    if( event->type() == LH::Event::InitChild::type() )
    {
        LH::Event::InitChild *e = static_cast<LH::Event::InitChild *>(event);
        QObjectList::const_iterator it;
        for( it = children().constBegin(); it != children().constEnd(); ++ it )
        {
            if( (*it)->objectName() == e->childName() )
            {
                if( Object * child = qobject_cast<Object *>(*it) )
                {
                    child->lh_init( e->id() );
                    return;
                }
            }
        }

        qCritical() << metaObject()->className()
                    << objectName()
                    << "LH::Event::InitChild: no child named"
                    << e->childName();
        return;
    }

    if( event->type() == LH::Event::SetProperty::type() )
    {
        LH::Event::SetProperty *e = static_cast<LH::Event::SetProperty *>(event);
        if( !setProperty( e->name(), e->value() ) )
        {
            if( metaObject()->indexOfProperty(e->name()) == -1 )
            {
                qCritical() << metaObject()->className()
                            << objectName()
                            << "LH::Event::SetProperty: failed to set"
                            << e->name();
            }
            else
            {
                qWarning() << metaObject()->className()
                           << objectName()
                           << "LH::Event::SetProperty: added dynamic"
                           << e->name();
            }
        }
        return;
    }

    qCritical() << metaObject()->className()
                << objectName()
                << "unhandled event"
                << LH::Event::typeName( event->type() );
    return;
}

QString Object::ident( const QString & str, const QObject *obj )
{
    QString retv(
                str.simplified()
                .replace('<','(')
                .replace('/','|')
                .replace('>',')')
                .replace('\"','\'')
                .replace('\\','|')
                );

    if( retv.isEmpty() && obj )
    {
        retv.append( obj->metaObject()->className() );
        retv.append( '@' );
        retv.append( QString::number((qptrdiff)obj,16) );
    }

    return retv;
}

} // namespace Lib6
} // namespace LH
