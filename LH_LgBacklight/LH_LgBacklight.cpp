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
"<p>HID-level driver for Logitech backlit devices, such as G13, G15 or G19. "
"Note that to use this driver you may need to uninstall existing drivers for these devices.</p>"
"<p>If there are no recognized devices connected, all you'll see is the 'Scan for available devices' "
"button. If you connect or disconnect backlit Logitech devices while this plugin is running, click "
"that button have them discovered.</p>"
"<p>To set the backlight for all connected devices from a layout, create a <i>data source</i> named "
"<tt>/plugin/backlight/all/set</tt> using the link button <img src=\":/LH_Backlight/source.png\"></img> at any color item.</p>"
  "</longdesc>"
"</lcdhostplugin>";

const char *LH_LgBacklight::userInit()
{
    if( const char *err = LH_QtPlugin::userInit() ) return err;
    devselect_ = new LH_Qt_QStringList( this, "Device", QStringList(), LH_FLAG_NOSAVE|LH_FLAG_NOSOURCE|LH_FLAG_NOSINK|LH_FLAG_HIDDEN );
    devselect_->setHelp( "The device which backlight you want to control." );
    connect( devselect_, SIGNAL(changed()), this, SLOT(changeDev()) );

    devcolor_ = new LH_Qt_QColor( this, "Color", Qt::white, LH_FLAG_NOSAVE|LH_FLAG_HIDDEN );
    devcolor_->setHelp("The color you want to see used as backlight.");
    connect( devcolor_, SIGNAL(changed()), this, SLOT(changeColor()) );

    allcolor_ = new LH_Qt_QColor( this, "SetAllColor", Qt::white, LH_FLAG_NOSAVE|LH_FLAG_HIDDEN|LH_FLAG_NOSOURCE );
    allcolor_->setLink("=/plugin/backlight/all/set");
    connect( allcolor_, SIGNAL(changed()), this, SLOT(setAllColor()) );

#ifdef lh_cb_reload
    rescanbutton_ = new LH_Qt_QString( this, "Rescan",tr("Reload to scan for new devices"),
                                       LH_FLAG_LAST|LH_FLAG_HIDETITLE|LH_FLAG_NOSAVE|LH_FLAG_NOSOURCE|LH_FLAG_NOSINK,
                                       lh_type_string_button );
    connect( rescanbutton_, SIGNAL(changed()), this, SLOT(wantRescan()) );
#endif

    hid_init();
    scan();
    return NULL;
}

void LH_LgBacklight::userTerm()
{
    hid_exit();
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
                // case 0xC222: /* G15 has no backlight control */
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
    else
        qDebug() << "LH_LgBacklight: hid_enumerate() failed";


    QString current = devselect_->valueText();
    QStringList sl;
    foreach( LgBacklightDevice *d, devs_ )
    {
        if( d->removal() )
        {
            devs_.removeAll(d);
            delete d;
        }
        else
            sl.append( d->name() );
    }

    devselect_->setVisible( devs_.size() > 0 );
    devcolor_->setVisible( devs_.size() > 0 );
    devselect_->setList(sl);
    devselect_->setValue(current);

    if( devselect_->index() < 0 && devselect_->list().size() > 0 )
        devselect_->setIndex(0);

    changeDev(); // make sure color value is up-to-date

    return;
}

void LH_LgBacklight::changeDev()
{
    foreach( LgBacklightDevice *d, devs_ )
    {
        if( d->name() == devselect_->valueText() )
        {
            devcolor_->setValue( d->color() );
            break;
        }
    }
}

void LH_LgBacklight::changeColor()
{
    foreach( LgBacklightDevice *d, devs_ )
    {
        if( d->name() == devselect_->valueText() )
        {
            d->setColor( devcolor_->value() );
            break;
        }
    }
}

void LH_LgBacklight::setAllColor()
{
    foreach( LgBacklightDevice *d, devs_ )
    {
        d->setColor( allcolor_->value() );
    }
}

void LH_LgBacklight::wantRescan()
{
#ifdef lh_cb_reload
    callback( lh_cb_reload, (void*) "rescan for devices" );
#endif
}
