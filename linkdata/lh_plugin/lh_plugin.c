/**
  \file     lh_plugin.c
  \author   Johan Lindh <johan@linkdata.se>
  \brief    Provides some utility functions for handling the lh_blob type.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lh_global.h"

/**
        lh_buffer_to_headerfile()

        Turns a lh_buffer structure into a C/C++ headerfile, suitable for inclusion.

        \param buffer
                The lh_buffer to convert.
	\param filename
		Name of the file to write to. Will be created if it doesn't exist,
		truncated and overwritten if it does exist.
	\param varname
                Name of the char array variable inside the created headerfile.

*/
void lh_buffer_to_headerfile( const lh_buffer_t *buffer, const char *filename, const char *varname )
{
    unsigned n, m;
    FILE *f;

    if( filename == NULL || buffer == NULL ) return;

    f = fopen( filename, "w" );
    if( f != NULL )
    {
        fprintf( f,
                "/* %s */\n"
                "\n"
                "const char %s[%d] = \n"
                "{\n"
                "        "
                "", filename, varname, buffer->n
                );

        for( m=n=0; n<buffer->n; n++ )
        {
            m += fprintf( f, "%u", buffer->p[n] );
            if( n<buffer->n-1 ) m += fprintf( f, "," );
            if( m > 60 )
            {
                fprintf( f, "\n        " );
                m = 0;
            }
        }

        fprintf( f,
                 "\n    }\n"
                 "};\n"
                 "/* eof */\n"
                 "\n" );
        fclose( f );
    }
    return;
}

lh_ui_type lh_name_to_userinterface( const char *name )
{
    if( name == 0 || !*name ) return lh_ui_none;
    if( !strcmp("button",name)) return lh_ui_button;
    if( !strcmp("checkbox",name)) return lh_ui_checkbox;
    if( !strcmp("color",name)) return lh_ui_color;
    if( !strcmp("combobox",name)) return lh_ui_combobox;
    if( !strcmp("dropdownbox",name)) return lh_ui_dropdownbox;
    if( !strcmp("filename",name)) return lh_ui_filename;
    if( !strcmp("font",name)) return lh_ui_font;
    if( !strcmp("htmllink",name)) return lh_ui_htmllink;
    if( !strcmp("image",name)) return lh_ui_image;
    if( !strcmp("input_state",name)) return lh_ui_input_state;
    if( !strcmp("input_value",name)) return lh_ui_input_value;
    if( !strcmp("listbox",name)) return lh_ui_listbox;
    if( !strcmp("none",name)) return lh_ui_none;
    if( !strcmp("progress",name)) return lh_ui_progress;
    if( !strcmp("slider",name)) return lh_ui_slider;
    if( !strcmp("spinbox",name)) return lh_ui_spinbox;
    if( !strcmp("string",name)) return lh_ui_string;
    if( !strcmp("text",name)) return lh_ui_text;
    return lh_ui_none;
}

const char *lh_userinterface_to_name( const lh_ui_type ui )
{
    switch( ui )
    {
    case lh_ui_none: return "none";
    case lh_ui_checkbox: return "checkbox";
    case lh_ui_color: return "color";
    case lh_ui_font: return "font";
    case lh_ui_image: return "image";
    case lh_ui_spinbox: return "spinbox";
    case lh_ui_slider: return "slider";
    case lh_ui_progress: return "progress";
    case lh_ui_string: return "string";
    case lh_ui_text: return "text";
    case lh_ui_filename: return "filename";
    case lh_ui_button: return "button";
    case lh_ui_htmllink: return "htmllink";
    case lh_ui_dropdownbox: return "dropdownbox";
    case lh_ui_listbox: return "listbox";
    case lh_ui_combobox: return "combobox";
    case lh_ui_input_value: return "input_value";
    case lh_ui_input_state: return "input_state";
    case lh_ui_type_unused:
    case lh_ui_type_mask:
        break;
    }
    return "(invalid)";
}

#if 0
lh_meta lh_name_to_data_format( const char *name )
{
    if( name == 0 || !*name ) return lh_format_none;
    if( !strcmp("boolean",name)) return lh_format_boolean;
    if( !strcmp("color",name)) return lh_format_color;
    if( !strcmp("double",name)) return lh_format_double;
    if( !strcmp("font",name)) return lh_format_font;
    if( !strcmp("input",name)) return lh_format_input;
    if( !strcmp("integer",name)) return lh_format_integer;
    if( !strcmp("none",name)) return lh_format_none;
    if( !strcmp("png",name)) return lh_format_png;
    if( !strcmp("local8",name)) return lh_format_local8;
    if( !strcmp("utf8",name)) return lh_format_utf8;
    if( !strcmp("list",name)) return lh_format_list;
    return lh_format_unused;
}

const char *lh_data_format_to_name( const lh_meta prop )
{
    switch( prop & lh_format_mask )
    {
    case lh_format_boolean:     return "boolean";
    case lh_format_color:       return "color";
    case lh_format_double:      return "double";
    case lh_format_font:        return "font";
    case lh_format_input:       return "input";
    case lh_format_integer:     return "integer";
    case lh_format_none:        return "none";
    case lh_format_png:         return "png";
    case lh_format_local8:      return "local8";
    case lh_format_utf8:        return "utf8";
    case lh_format_list:        return "list";
    case lh_format_unused:
        break;
    }
    return "(invalid)";
}
#endif
