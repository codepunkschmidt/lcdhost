/**
  \file     LH_QtSetupItem.cpp
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

#include "LH_QtSetupItem.h"

LH_QtSetupItem::LH_QtSetupItem( LH_QtObject *parent, QString name, lh_setup_type type, int flags ) :
    lh_setup( *parent, name, lh_ui_none|lh_meta_default ),
    api5type_( lh_type_none ),
    api5flags_( 0 )
{
    setType( type );
    setFlags( flags );
}

void LH_QtSetupItem::setType( lh_setup_type t )
{
    int newmeta = meta() & lh_ui_flag_mask;

    switch( t )
    {
    case lh_type_none:              newmeta |= lh_ui_none; break;
    case lh_type_integer:           newmeta |= lh_ui_spinbox; break;
    case lh_type_integer_boolean:   newmeta |= lh_ui_checkbox; break;
    case lh_type_integer_color:     newmeta |= lh_ui_color; break;
    case lh_type_integer_slider:    newmeta |= lh_ui_slider; break;
    case lh_type_integer_progress:  newmeta |= lh_ui_progress; break;
    case lh_type_integer_list:      newmeta |= lh_ui_dropdownbox; break;
    case lh_type_fraction:          newmeta |= lh_ui_spinbox; break;
    case lh_type_string:            newmeta |= lh_ui_string; break;
    case lh_type_string_script:     newmeta |= lh_ui_text; break;
    case lh_type_string_filename:   newmeta |= lh_ui_filename; break;
    case lh_type_string_font:       newmeta |= lh_ui_font; break;
    case lh_type_string_inputstate: newmeta |= lh_ui_input_state; break;
    case lh_type_string_inputvalue: newmeta |= lh_ui_input_value; break;
    case lh_type_image_png:         newmeta |= lh_ui_image; break;
    case lh_type_image_qimage:      newmeta |= lh_ui_image; break;
    case lh_type_integer_listbox:   newmeta |= lh_ui_listbox; break;
    case lh_type_string_button:     newmeta |= lh_ui_button; break;
    case lh_type_string_html:       newmeta |= lh_ui_htmllink; break;
    case lh_type_last:
    default:
        Q_ASSERT(0);
        newmeta |= lh_ui_none;
        break;
    }

    setMeta( newmeta );
}

void LH_QtSetupItem::setFlags( int f )
{
    int newmeta = ui() | lh_meta_default;

    if( f & LH_FLAG_READONLY    ) ;
    if( f & LH_FLAG_HIDDEN      ) ;
    if( f & LH_FLAG_FOCUS       ) ;
    if( f & LH_FLAG_AUTORENDER  ) ;
    if( f & LH_FLAG_FIRST       ) ;
    if( f & LH_FLAG_LAST        ) ;
    if( f & LH_FLAG_NOSAVE      ) ;
    if( f & LH_FLAG_BLANKTITLE  ) ;
    if( f & LH_FLAG_NOSOURCE    ) ;
    if( f & LH_FLAG_NOSINK      ) ;
    if( f & LH_FLAG_HIDETITLE   ) ;
    if( f & LH_FLAG_HIDEVALUE   ) ;

    setMeta( newmeta );
}

// Using the '=' and '@' prefixes
void LH_QtSetupItem::setLink(QString s)
{
    setPush();
    setPull();
    if( s.startsWith('=') )
    {
        s.remove(0,1);
        setPull(s);
    }
    else if( s.startsWith('@') )
    {
        s.remove(0,1);
        setPush(s);
    }
    return;
}

QString LH_QtSetupItem::link()
{
    if( !push().isEmpty() ) return QString('@').append(push());
    if( !pull().isEmpty() ) return QString('=').append(pull());
    return QString();
}

void LH_QtSetupItem::setup_change()
{
    emit changed();
}
