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
#include "LH_LgBacklight.h"

LH_PLUGIN(LH_LgBacklight)

char __lcdhostplugin_xml[] =
"<?xml version=\"1.0\"?>"
"<lcdhostplugin>"
  "<id>Backlight</id>"
  "<rev>" STRINGIZE(REVISION) "</rev>"
  "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
  "<ver>" "r" STRINGIZE(REVISION) "</ver>"
  "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
  "<author>Johan \"SirReal\" Lindh</author>"
  "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
  "<logourl></logourl>"
  "<shortdesc>"
  "Logitech device backlight control via HID"
  "</shortdesc>"
  "<longdesc>"
    "HID-level driver for Logitech backlit devices, such as G13, G15 or G19.<br/>"
    "Note that to use this driver you may need to uninstall existing drivers for these devices."
  "</longdesc>"
"</lcdhostplugin>";

const char *LH_LgBacklight::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;
    devselect_ = new LH_Qt_QStringList( this, "Device", QStringList("Default"), LH_FLAG_NOSAVE );
    connect( devselect_, SIGNAL(changed()), this, SLOT(changeDev()) );
    devcolor_ = new LH_Qt_QColor( this, "Color", Qt::white, LH_FLAG_NOSAVE );
    connect( devcolor_, SIGNAL(changed()), this, SLOT(changeColor()) );
    devisdefault_ = new LH_Qt_bool( this, "Default",false,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    connect( devisdefault_, SIGNAL(changed()), this, SLOT(defaultCheckbox()) );
    rescanbutton_ = new LH_Qt_QString( this, "Rescan","Rescan",
                                       LH_FLAG_LAST|LH_FLAG_NOSAVE|LH_FLAG_NOSOURCE, lh_type_string_button );
    defaultdev_ = new LH_Qt_QString( this, "DefaultDevice", QString(), LH_FLAG_HIDDEN );
    connect( defaultdev_, SIGNAL(changed()), this, SLOT(changeDev()) );
    connect( rescanbutton_, SIGNAL(changed()), this, SLOT(scan()) );
    changeDev();
    scan();
    return NULL;
}

void LH_LgBacklight::scan()
{
    // Maintain list of available devices
    foreach( LgBacklightDevice *d, devs_ ) d->setRemoval( true );

    if( struct hid_device_info *hdi_head = hid_enumerate( 0x0, 0x0 ) )
    {
        for( struct hid_device_info *hdi = hdi_head; hdi; hdi = hdi->next )
        {
            if( hdi->vendor_id == 0x046d )
            {
                switch( hdi->product_id )
                {
                case 0xC222: /* G15 */
                case 0xC225: /* G11 */
                case 0xC227: /* G15v2 */
                case 0xC21C: /* G13 */
                case 0xC22D: /* G510 without audio */
                case 0xC22E: /* G510 with audio */
                case 0xC229: /* G19 */
                    {
                        bool found = false;
                        foreach( LgBacklightDevice *d, devs_ )
                        {
                            if( d->path() == hdi->path )
                            {
                                d->setRemoval( false );
                                found = true;
                                break;
                            }
                        }
                        if( !found ) devs_.append( new LgBacklightDevice(hdi,this) );
                    }
                    break;
                case 0x0A07: /* Z10 */
                default:
                    break;
                }
            }
        }
        hid_free_enumeration( hdi_head );
    }

    devselect_->list().clear();
    devselect_->list().append("Default");
    foreach( LgBacklightDevice *d, devs_ )
    {
        if( d->removal() )
        {
            devs_.removeAll(d);
            delete d;
        }
        else
        {
            devselect_->list().append( d->name() );
        }
    }
    devselect_->refreshList();

    return;
}

void LH_LgBacklight::changeDev()
{
    QColor usecolor;
    devisdefault_->setHidden( devselect_->value() < 1 );
    devisdefault_->setValue( devselect_->valueText() == defaultdev_->value() );
    foreach( LgBacklightDevice *d, devs_ )
    {
        if( d->name() == devselect_->valueText() ||
            (d->name() == defaultdev_->value() && !usecolor.isValid()) )
            usecolor = d->color();
    }
    devcolor_->setValue( usecolor );
}

void LH_LgBacklight::changeColor()
{
    QString lookfor;
    if( devselect_->value() < 1 ) lookfor = defaultdev_->value();
    else lookfor = devselect_->valueText();

    foreach( LgBacklightDevice *d, devs_ )
    {
        if( d->name() == lookfor )
        {
            d->setColor( devcolor_->value() );
            break;
        }
    }
}

void LH_LgBacklight::defaultCheckbox()
{
    if( devisdefault_->value() )
        defaultdev_->setValue( devselect_->valueText() );
    else
        defaultdev_->setValue( QString() );
}
