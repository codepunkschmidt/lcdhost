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

lh_meta_type lh_name_to_meta_type( const char *name )
{
    if( name == 0 || !*name ) return lh_type_none;

    switch( *name )
    {
    case 'a':
        if( !strcmp(name,"array") ) return lh_type_array;
        break;
    case 'b':
        if( !strcmp(name,"boolean") ) return lh_type_boolean;
        if( !strcmp(name,"button") ) return lh_type_string_button;
        if( !strcmp(name,"buffer") ) return lh_type_buffer;
        break;
    case 'c':
        if( !strcmp(name,"color") ) return lh_type_color;
        if( !strcmp(name,"combobox") ) return lh_type_string_combobox;
        break;
    case 'd':
        if( !strcmp(name,"double") ) return lh_type_double;
        if( !strcmp(name,"dropdownbox") ) return lh_type_string_dropdownbox;
        break;
    case 'f':
        if( !strcmp(name,"filename") ) return lh_type_string_filename;
        if( !strcmp(name,"font") ) return lh_type_font;
        break;
    case 'h':
        if( !strcmp(name,"htmllink") ) return lh_type_string_htmllink;
        break;
    case 'i':
        if( !strcmp(name,"integer") ) return lh_type_integer;
        if( !strcmp(name,"inputstate") ) return lh_type_input_state;
        if( !strcmp(name,"inputvalue") ) return lh_type_input_value;
        break;
    case 'l':
        if( !strcmp(name,"listbox") ) return lh_type_string_listbox;
        break;
    case 'n':
        if( !strcmp(name,"none") ) return lh_type_none;
        break;
    case 'p':
        if( !strcmp(name,"png") ) return lh_type_png;
        if( !strcmp(name,"progress") ) return lh_type_integer_progress;
        if( !strcmp(name,"pointer") ) return lh_type_pointer;
        break;
    case 'q':
        if( !strcmp(name,"qimage") ) return lh_type_pointer_qimage;
        break;
    case 's':
        if( !strcmp(name,"string") ) return lh_type_string;
        if( !strcmp(name,"slider") ) return lh_type_integer_slider;
        if( !strcmp(name,"script") ) return lh_type_string_script;
        break;
    }

    return lh_meta_type_unused;
}

const char *lh_meta_type_to_name( const lh_meta_type t )
{
    switch( t )
    {
    case lh_type_array:             return "array";
    case lh_type_boolean:           return "boolean";
    case lh_type_buffer:            return "buffer";
    case lh_type_string_button:     return "button";
    case lh_type_color:             return "color";
    case lh_type_string_combobox:   return "combobox";
    case lh_type_double:            return "double";
    case lh_type_string_dropdownbox:return "dropdownbox";
    case lh_type_string_filename:   return "filename";
    case lh_type_font:              return "font";
    case lh_type_string_htmllink:   return "htmllink";
    case lh_type_input_state:       return "inputstate";
    case lh_type_input_value:       return "inputvalue";
    case lh_type_integer:           return "integer";
    case lh_type_string_listbox:    return "listbox";
    case lh_type_none:              return "none";
    case lh_type_png:               return "png";
    case lh_type_pointer:           return "pointer";
    case lh_type_integer_progress:  return "progress";
    case lh_type_pointer_qimage:    return "qimage";
    case lh_type_string_script:     return "script";
    case lh_type_integer_slider:    return "slider";
    case lh_type_string:            return "string";

    case lh_meta_type_unused:
        break;
    }

    return "(invalid)";
}

lh_format lh_name_to_format( const char *name )
{
    if( name == 0 || !*name ) return lh_format_none;

    switch( *name )
    {
    case 'a':
        if( !strcmp("array",name) ) return lh_format_array;
        break;
    case 'b':
        if( !strcmp("boolean",name) ) return lh_format_boolean;
        if( !strcmp("buffer",name) ) return lh_format_buffer;
        break;
    case 'c':
        if( !strcmp("color",name) ) return lh_format_color;
        break;
    case 'd':
        if( !strcmp("double",name) ) return lh_format_double;
        break;
    case 'f':
        if( !strcmp("font",name) ) return lh_format_font;
        break;
    case 'i':
        if( !strcmp("integer",name) ) return lh_format_integer;
        if( !strcmp("input",name) ) return lh_format_input;
        break;
    case 'n':
        if( !strcmp("none",name) ) return lh_format_none;
        break;
    case 'p':
        if( !strcmp("png",name) ) return lh_format_png;
        if( !strcmp("pointer",name) ) return lh_format_pointer;
        break;
    case 'q':
        if( !strcmp("qimage",name) ) return lh_format_qimage;
        break;
    case 's':
        if( !strcmp("string",name) ) return lh_format_string;
        if( !strcmp("store_buffer",name) ) return lh_store_buffer;
        if( !strcmp("store_double",name) ) return lh_store_double;
        if( !strcmp("store_integer",name) ) return lh_store_integer;
        if( !strcmp("store_pointer",name) ) return lh_store_pointer;
        break;
    }

    return lh_format_unused;
}

const char *lh_format_to_name( const lh_format fmt )
{
    switch( fmt )
    {
    case lh_format_array:   return "array";
    case lh_format_boolean: return "boolean";
    case lh_format_buffer:  return "buffer";
    case lh_format_color:   return "color";
    case lh_format_double:  return "double";
    case lh_format_font:    return "font";
    case lh_format_input:   return "input";
    case lh_format_integer: return "integer";
    case lh_format_none:    return "none";
    case lh_format_png:     return "png";
    case lh_format_pointer: return "pointer";
    case lh_format_qimage:  return "qimage";
    case lh_store_buffer:   return "store_buffer";
    case lh_store_double:   return "store_double";
    case lh_store_integer:  return "store_integer";
    case lh_store_pointer:  return "store_pointer";
    case lh_format_string:  return "string";

    case lh_format_unused:
        break;
    }

    return "(invalid)";
}
