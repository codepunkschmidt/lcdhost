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

#include "hidapi/hidapi.h"
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
#if 0
            if( hdi )
            {
                hid_device *dev = hid_open_path( hdi->path );
                if( dev )
                {
                    unsigned char last_keys[LG_KEY_MAX];
                    memset( last_keys, 0, sizeof(last_keys) );
                    hid_set_nonblocking( dev, 1 );
                    data->offline = 0;

                    /* Run update cycle until an error occurs */
                    while( !data->time_to_die && !data->offline )
                    {
                        if( WaitForSingleObject( data->semaphore, 20 ) == WAIT_OBJECT_0 ) // 50 FPS should be enough for most ppl
                        {
                            if( data->updaterequest )
                            {
                                if( MTX_ERR(WaitForSingleObject( data->mutex, 1000 )) )
                                {
                                    unsigned char buffer[ G15_BUFFER_LEN ];
                                    make_output_report( buffer, data->pixels );
                                    ReleaseMutex( data->mutex );
                                    if( hid_write( dev, buffer, sizeof(buffer) ) != sizeof(buffer) ) data->offline = 1;
                                    else data->updaterequest = 0;
                                }
                            }
                            if( !data->offline )
                            {
                                unsigned char buffer[G15_KEY_READ_LENGTH];
                                retv = hid_read( dev, buffer, sizeof(buffer) );
                                while( retv > 0 )
                                {
                                    int deltas = 0;
                                    INPUT *send_list = 0;
                                    unsigned char keys[LG_KEY_MAX];
                                    lcd_gkeys( buffer, retv, keys );

                                    lcd_button_cb( 0,
                                                   ( keys[LG_KEY_L2] ? LGLCDBUTTON_BUTTON0 : 0 ) |
                                                   ( keys[LG_KEY_L3] ? LGLCDBUTTON_BUTTON1 : 0 ) |
                                                   ( keys[LG_KEY_L4] ? LGLCDBUTTON_BUTTON2 : 0 ) |
                                                   ( keys[LG_KEY_L5] ? LGLCDBUTTON_BUTTON3 : 0 ),
                                                   data );

                                    for( index = 0; index <= LG_KEY_MAX; ++ index )
                                    {
                                        if( keys[index] != last_keys[index] &&
                                            lcd_gkeys_scancode( index, keys[index] ) ) ++ deltas;
                                    }

                                    if( deltas )
                                    {
                                        int count = 0;
                                        send_list = new INPUT[deltas];
                                        for( index = 0; index < LG_KEY_MAX; ++ index )
                                        {
                                            if( last_keys[index] != keys[index] &&
                                                lcd_gkeys_scancode( index, keys[index] )
                                                )
                                            {
                                                send_list[count].type = INPUT_KEYBOARD;
                                                send_list[count].ki.wVk = 0;
                                                send_list[count].ki.wScan = lcd_gkeys_scancode( index, keys[index] );
                                                send_list[count].ki.dwFlags = KEYEVENTF_SCANCODE;
                                                send_list[count].ki.time = 0;
                                                send_list[count].ki.dwExtraInfo = 0;
                                                if( !keys[index] )
                                                    send_list[count].ki.dwFlags |= KEYEVENTF_KEYUP;
                                                ++ count;
                                            }
                                        }
                                        if( count == deltas )
                                        {
                                            count = SendInput( deltas, send_list, sizeof(INPUT) );
                                            if( deltas != count )
                                            {
                                                log("failed to insert all events, %d of %d", count, deltas );
                                            }
                                        }
                                        delete[] send_list;
                                    }
                                    memcpy( last_keys, keys, sizeof(last_keys) );
                                    retv = hid_read( dev, buffer, sizeof(buffer) );
                                }
                            }
                        }
                    }
                    hid_close( dev );
                }
            }
            hid_free_enumeration( hdi_head );
#endif
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
