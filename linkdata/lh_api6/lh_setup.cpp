#include "lh_setup.h"

using namespace lh::api6;

setup::ui_type setup::ui_name( const char * name )
{
    if( name == 0 || !*name ) return ui_none;
    if( !strcmp("button",name)) return ui_button;
    if( !strcmp("checkbox",name)) return ui_checkbox;
    if( !strcmp("color",name)) return ui_color;
    if( !strcmp("combobox",name)) return ui_combobox;
    if( !strcmp("dropdownbox",name)) return ui_dropdownbox;
    if( !strcmp("filename",name)) return ui_filename;
    if( !strcmp("font",name)) return ui_font;
    if( !strcmp("htmllink",name)) return ui_htmllink;
    if( !strcmp("image",name)) return ui_image;
    if( !strcmp("input_state",name)) return ui_input_state;
    if( !strcmp("input_value",name)) return ui_input_value;
    if( !strcmp("listbox",name)) return ui_listbox;
    if( !strcmp("none",name)) return ui_none;
    if( !strcmp("progress",name)) return ui_progress;
    if( !strcmp("slider",name)) return ui_slider;
    if( !strcmp("spinbox",name)) return ui_spinbox;
    if( !strcmp("string",name)) return ui_string;
    if( !strcmp("text",name)) return ui_text;
    return ui_none;
}

const char *setup::ui_name( ui_type t )
{
    switch( t )
    {
    case ui_none: return "none";
    case ui_checkbox: return "checkbox";
    case ui_color: return "color";
    case ui_font: return "font";
    case ui_image: return "image";
    case ui_spinbox: return "spinbox";
    case ui_slider: return "slider";
    case ui_progress: return "progress";
    case ui_string: return "string";
    case ui_text: return "text";
    case ui_filename: return "filename";
    case ui_button: return "button";
    case ui_htmllink: return "htmllink";
    case ui_dropdownbox: return "dropdownbox";
    case ui_listbox: return "listbox";
    case ui_combobox: return "combobox";
    case ui_input_value: return "input_value";
    case ui_input_state: return "input_state";
    case ui_type_mask:
        break;
    }
    return "(invalid)";
}

void setup::setMeta( int meta )
{
    if( meta_ == meta ) return;
    meta_ = meta;
    emit metaChanged( meta_ );
}

void setup::setOrder( int order )
{

}

void setup::setHelp( const QString & help )
{

}

void setup::setList( const QStringList & sl )
{

}
