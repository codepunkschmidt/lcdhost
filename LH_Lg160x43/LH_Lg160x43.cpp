/**
  \file     LH_Lg160x43.cpp
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

#include <QFile>
#include <QDebug>

#include "hidapi.h"
#include "../LH_QtDevice.h"
#include "LH_Lg160x43.h"
#include "Lg160x43Device.h"
#include "../LH_Qt_QImage.h"

LH_Lg160x43 theDriver;

int LH_Lg160x43::lh_notify(int note, void*param)
{
    if( !note || note&LH_NOTE_SECOND )
    {
        // Maintain list of available devices
        struct hid_device_info *hdi_head = hid_enumerate( 0x046d, 0x0 );
        if( hdi_head )
        {
            struct hid_device_info *hdi = 0;

            foreach( QObject *kid, children() )
            {
                Lg160x43Device *d = qobject_cast<Lg160x43Device*>(kid);
                if( d ) d->setRemoval( true );
            }

            for( hdi = hdi_head; hdi; hdi = hdi->next )
            {
                if( hdi->product_id == 0xC222 /* G15 */ ||
                    hdi->product_id == 0x0A07 /* Z10 */ ||
                    hdi->product_id == 0xC227 /* G15v2 */ ||
                    hdi->product_id == 0xC21C /* G13 */ ||
                    hdi->product_id == 0xC22D /* G510 without audio */ ||
                    hdi->product_id == 0xC22E /* G510 with audio */
                    )
                {
                    bool found = false;
                    foreach( QObject *kid, children() )
                    {
                        Lg160x43Device *d = qobject_cast<Lg160x43Device*>(kid);
                        if( d && d->path() == hdi->path )
                        {
                            Q_ASSERT( hdi->product_id == d->productId() );
                            Q_ASSERT( d->removal() == true );
                            d->setRemoval( false );
                            found = true;
                        }
                    }
                    if( !found )
                    {
                        new Lg160x43Device( hdi, this );
                    }
                }
            }

            hid_free_enumeration( hdi_head );

            foreach( QObject *kid, children() )
            {
                Lg160x43Device *d = qobject_cast<Lg160x43Device*>(kid);
                if( d && d->removal() )
                {
                    delete d;
                }
            }
        }
    }
    return LH_NOTE_SECOND;
}

#if 0
const char *LH_Lg160x43::lh_load()
{
    return NULL;
}
#endif
