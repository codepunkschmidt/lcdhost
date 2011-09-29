
#include <stdio.h>
#include <string.h>
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
    int n, m;
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
