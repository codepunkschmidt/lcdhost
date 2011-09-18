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

#include "lh_plugin.h"

/**
	lh_binaryfile_to_blob()

	Reads a file and converts it into a lh_blob structure.

	\param filename
		Name of the file to load into a blob.
	\return
		A pointer to a malloc()'d lh_blob. The caller must
		free this when done with it.
*/
lh_blob *lh_binaryfile_to_blob( const char *filename )
{
    lh_blob *blob = NULL;
    long filesize = 0;
    FILE *f;

    if( filename == NULL ) return NULL;

    f = fopen( filename, "rb" );
    if( f != NULL )
    {
        if( fseek( f, 0, SEEK_END ) == 0 ) filesize = ftell( f );
        if( filesize > 0 )
        {
            fseek( f, 0, SEEK_SET );
            blob = malloc( sizeof(lh_blob) + filesize );
            if( blob )
            {
                blob->sign = 0xDEADBEEF;
                blob->len = filesize;
                if( fread( & blob->data, 1, filesize, f ) != (size_t) filesize )
                {
                    blob->sign = 0;
                    blob->len = 0;
                    free( blob );
                    blob = NULL;
                }
            }
        }
        fclose( f );
    }

    return blob;
}

/**
	lh_blob_to_headerfile()

	Turns a lh_blob structure into a C/C++ headerfile, suitable for inclusion.

	\param blob
		The blob to convert.
	\param filename
		Name of the file to write to. Will be created if it doesn't exist,
		truncated and overwritten if it does exist.
	\param varname
		Name of the lh_blob variable inside the created headerfile.

*/
void lh_blob_to_headerfile( lh_blob *blob, const char *filename, const char *varname )
{
    unsigned n, m;
    FILE *f;

    if( filename == NULL || blob == NULL ) return;

    f = fopen( filename, "w" );
    if( f != NULL )
    {
        fprintf( f,
                "/* %s */\n"
                "\n"
                "struct _%s_s { const unsigned len; const unsigned sign; const unsigned char data[%u]; };\n"
                "struct _%s_s const _%s_d = \n"
                "{\n"
                "    %u, 0x%X,\n"
                "    {\n"
                "        "
                "", filename, varname, blob->len, varname, varname, blob->len, blob->sign
                );

        for( m=n=0; n<blob->len; n++ )
        {
            m += fprintf( f, "%u", blob->data[n] );
            if( n<blob->len-1 ) m += fprintf( f, "," );
            if( m > 60 )
            {
                fprintf( f, "\n        " );
                m = 0;
            }
        }

        fprintf( f,
                 "\n    }\n"
                 "};\n"
                 "const void *%s = &_%s_d;\n"
                 "\n"
                 "/* eof */\n"
                 "\n", varname, varname );
        fclose( f );
    }
    return;
}

lh_setup_ui lh_name_to_setup_ui( const char *name )
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
    return lh_setup_ui_unused;
}

const char *lh_setup_ui_to_name( const lh_setup_ui t )
{
    switch( t )
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
    case lh_setup_ui_unused:
        break;
    }
    return "(invalid)";
}

lh_data_format lh_name_to_data_format( const char *name )
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
    if( !strcmp("string",name)) return lh_format_string;
    if( !strcmp("stringlist",name)) return lh_format_stringlist;
    return lh_format_unused;
}

const char *lh_data_format_to_name( const lh_data_format fmt )
{
    switch( fmt )
    {
    case lh_format_boolean:     return "boolean";
    case lh_format_color:       return "color";
    case lh_format_double:      return "double";
    case lh_format_font:        return "font";
    case lh_format_input:       return "input";
    case lh_format_integer:     return "integer";
    case lh_format_none:        return "none";
    case lh_format_png:         return "png";
    case lh_format_string:      return "string";
    case lh_format_stringlist:  return "stringlist";
    case lh_format_unused:
        break;
    }
    return "(invalid)";
}
