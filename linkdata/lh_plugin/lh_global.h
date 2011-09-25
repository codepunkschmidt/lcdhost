/**
  \file     lh_global.h
  \author   Johan Lindh <johan@linkdata.se>
  \brief    Global definitions for LCDHost plugins
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

/**
  These are the entry points for LCDHost plugins using the Qt C++ API.
  See lh_plugin.h for the C API.

  1.  If the plugin has a signature area, integrity is checked.
  2.  LCDHost examines the plugin's embedded XML document.
  3.  Either the user decides to load the plugin or it's set to auto load
  4.  LCDHost has the operating system load the shared library
  5.  \c lh_create() is called returning the plugin's \c lh_object_t*
  6.  \c obj_init() is called if it is provided by the plugin's \c lh_object_t*
    * at this point, barring errors, the plugin is considered loaded
    * when the user decides to unload the plugin or LCDHost shuts down:
  7.  \c lh_destroy() is called if it's provided by the plugin
  8.  LCDHost has the operating system unload the shared library

  lh_object_t *lh_create()
    Create the plugin object and return a lh_object_t pointer.
    LCDHost will provide this pointer when calling the functions
    in lh_object_t structure. Return NULL on error. The returned
    pointer must remain valid until lh_destroy() returns.

  void lh_destroy( lh_object_t *obj )
    Free resources associated with 'obj'. The shared library is about to be
    removed from memory. While this function is optional, it's
    recommended to use it to free up resources used by the plugin
    in an orderly fashion. Once lh_destroy() is completed, it's no
    longer safe for any of the plugin code to execute. Take care
    to join or terminate any threads you've started.

  LH_QObject *lh_qt_create()
    Instantiate a LH_QObject and return it if you're using
    Qt and the LCDHost Qt support classes. LCDHost will
    delete the object before unloading the shared library.
    See lh_qobject.h for further documentation.

  */


#ifndef LH_GLOBAL_H
#define LH_GLOBAL_H

#define LH_API_MAJOR 6
#define LH_API_MINOR 0

#ifndef NEXTBIT
# define NEXTBIT(v) (((v-1)>>1)|((v-1)>>2)|((v-1)>>4)|((v-1)>>8)|((v-1)>>16))
#endif

#ifndef STRINGIZE
# define STRINGIZE_(x) #x
# define STRINGIZE(x) STRINGIZE_(x)
#endif

#define LH_MAX_IDENT            64

#ifndef LH_ALIGN_LEFT
# define LH_ALIGN_LEFT       0
# define LH_ALIGN_CENTER     1
# define LH_ALIGN_RIGHT      2
# define LH_ALIGN_TOP        LH_ALIGN_LEFT
# define LH_ALIGN_BOTTOM     LH_ALIGN_RIGHT
# define LH_ALIGN_MAX        LH_ALIGN_RIGHT
#endif

#ifndef LH_METHOD_ADJUST
# define LH_METHOD_ADJUST    0   /**< add size offset size source */
# define LH_METHOD_PERMILLE  1   /**< make size X 1/1000:s of source (e.g. 1000 = same as source) */
# define LH_METHOD_LEFT      2   /**< adjust size so our right edge is the same as source left/top */
# define LH_METHOD_RIGHT     3   /**< adjust size so our right edge is the same as source right/bottom */
# define LH_METHOD_ABSOLUTE  4   /**< size is set to whatever offset is set to */
# define LH_METHOD_TOP       LH_METHOD_LEFT
# define LH_METHOD_BOTTOM    LH_METHOD_RIGHT
# define LH_METHOD_MAX       LH_METHOD_ABSOLUTE
#endif

/**
  Event types.
  */
typedef enum lh_eventcode_t
{
    lh_event_none = 0,

    /**
      The output device the layout is attached to has been changed.
      */
    lh_event_changed_output,

    /**
      The object's 'link' property and one or more properties exported
      with \c lh_link_source duplicate preexisting data sources with
      the same 'link' value and exported property roles.
      */
    lh_event_duplicate_link_source,

    /**
      Device input matching the 'value' property received, param: const lh_input*
      */
    lh_event_input,

    /**
      The object and all it's children have completed obj_init() ok.
      */
    lh_event_initialized,
    lh_event_unused
} lh_eventcode;

/**
  User interface flags for setup items.
  */
typedef enum lh_ui_flag_t
{
    lh_ui_stored = (1<<0), /**< this setup item should be saved if modified */
    lh_ui_visible = (1<<1), /**< show the setup item in the UI */
    lh_ui_enabled = (1<<2), /**< enable the setup item in the UI */
    lh_ui_source = (1<<3), /**< allow the UI to use this setup item as a data source */
    lh_ui_sink = (1<<4), /**< allow the UI to use this setup item as a data sink */
    lh_ui_focus = (1<<5), /**< this setup item actively wants focus */
    lh_ui_first = (1<<6), /**< same as setOrder(-1) */
    lh_ui_last = (1<<7), /**< same as setOrder(1) */
    lh_ui_flag_mask = NEXTBIT(lh_ui_focus)-1
} lh_ui_flag;

/**
  User interface type for setup items.
  */
typedef enum lh_ui_type_t
{
    lh_ui_none = (1<<16),
    lh_ui_checkbox, /**< checkbox */
    lh_ui_color, /**< line editor with color picker */
    lh_ui_font, /**< line editor with font picker */
    lh_ui_image, /**< decode PNG data and show the image */
    lh_ui_spinbox, /**< line editor with spinbox (optional min/max) */
    lh_ui_slider, /**< slider (required min/max) */
    lh_ui_progress, /**< progress bar (optional min/max) */
    lh_ui_string, /**< line editor */
    lh_ui_text, /**< multiline text editor */
    lh_ui_filename, /**< line editor and file selection dialog (optional other: string: file selection mask) */
    lh_ui_button, /**< a button (required other: string: button text) */
    lh_ui_htmllink, /**< clicked link in title text text */
    lh_ui_dropdownbox, /**< dropdown box (required other: stringlist: selectable values) */
    lh_ui_listbox, /**< listbox (required other: stringlist: selectable values) */
    lh_ui_combobox, /**< combobox (optional other: stringlist: selectable values) */
    lh_ui_input_value, /**< readonly text and input selection dialog for button or axis */
    lh_ui_input_state, /**< readonly text and input selection dialog for button state or half-axis */
    lh_ui_type_unused,
    lh_ui_type_mask = ~(lh_ui_none-1)
} lh_ui_type;

const int lh_meta_default = (lh_ui_stored|lh_ui_visible|lh_ui_enabled|lh_ui_source|lh_ui_sink);
typedef int lh_meta;

/**
  Used with the callback function.
  \sa lh_callback_t
  \sa lh_object_t.cb_fn
  */
typedef enum lh_callbackcode_t
{
    /* does not require a callback id */
    lh_cb_load_layout, /* request loading of a new layout, param: UTF-8 encoded file name relative to data path */

    /* sent from anything */
    lh_cb_set_property, /**< A primitive property has changed, param: \c const \c lh_property* */
    lh_cb_unload, /* ask that the plugin be unloaded, param: NULL or const char *message */
    lh_cb_reload, /* request the plugin reloaded, param: NULL or const char *message */
    lh_cb_log, /* add an UTF-8 encoded HTML string in the LCDHost log */
    lh_cb_polling, /* ask for a call to the polling function, param: NULL */

    /* sent from a lh_object_t, these create children of that object */
    lh_cb_setup_create, /* create a new setup item, param: lh_setup_item* */
    lh_cb_output_create, /* a new output device have been detected, param: lh_output_device* */
    lh_cb_input_create, /* a new input device have been detected, param: lh_input_device* */
    lh_cb_class_create, /* a new layout class is available, param: lh_layout_class* */

    /* may be sent from any object, but usually sent from input devices */
    lh_cb_input, /* an input device state has changed, param is pointer to lh_input */

    lh_cb_new, /* a lh::object derived item is being constructed, param: lh::object* */
    lh_cb_delete, /* a lh::object is being deleted, param: lh::object* */

    /* these remove a plugin created item */
    lh_cb_destroy, /* destroy a setup item, layout class, or device */

    /* sent from lh_layout_item */
    lh_cb_render, /* ask for a rendering sequence (prerender/width/height/render), param: NULL */

    lh_cb_unused
} lh_callbackcode;

/**
  Plugin-to-LCDHost callback function type.
  */
typedef int (*lh_callback_t)( const void *cb_id, lh_callbackcode code, void *param );

/**
  The input flags are used to describe an input device
  and it's events in general.
  */
typedef enum lh_input_flag_t
{
    lh_input_usage      = 0x0001, /**< input comes from a button (a 'usage' in HID terms) */
    lh_input_value      = 0x0002, /**< input comes from an axis (a 'value' in HID terms) */
    lh_input_pos        = 0x0004, /**< button pressed or positive axis values */
    lh_input_neg        = 0x0008, /**< button released or negative axis values */

    lh_input_button     = lh_input_usage,
    lh_input_axis       = lh_input_value,
    lh_input_pressed    = lh_input_pos,
    lh_input_released   = lh_input_neg,

    lh_input_state_mask = (lh_input_usage|lh_input_value|lh_input_pos|lh_input_neg),

    lh_input_keyboard    = 0x1000, /**< this is a keyboard type device */
    lh_input_mouse       = 0x2000, /**< this is a mouse type device */
    lh_input_joystick    = 0x4000, /**< this is a joystick type device */
    lh_input_other       = 0x8000, /**< this is another of type device */

    lh_input_device_mask = (lh_input_keyboard|lh_input_mouse|lh_input_joystick|lh_input_other),

    lh_input_unused
} lh_input_flag;

/**
  Used when the state of an input device (button, slider, stick, whatever) changes.

  \c ident must be a globally unique id for the device. A suggested format is
  to use the HID codes in four-hexadecimal digits groups separated by colons:
    vendor:product:version:page:usage

  \c item is used to uniquely identify the control for that device. Together
  with ident, forms a string uniquely identifying the input control in the format
  "ident/+item", which is used when storing references to the control in
  setup items. Note that the 'item' value is considered a signed integer, and
  the sign is always encoded ('+' or '-'). In case of a specific device state
  being encoded, '/flags' are appended.

  The lh_input_device which does the lh_cb_input callback with this structure
  should export the obj_input_name() function if you want human readable device
  and device event names.
  */
typedef struct lh_input_t
{
    char ident[LH_MAX_IDENT]; /**< device identity, \sa lh_object_t.ident */
    int item; /**< control item identifier, must not be zero */
    int flags; /**< describes kind of control and basic state, \sa lh_input_flag */
    int value; /**< the exact value of the control item */
} lh_input;

/**
    Definition of the signature area
    The signature area is optional by highly recommended.
    Note that the signature needs to kept in the library. If
    it's not marked as an export, and not referenced anywhere,
    smart linkers are liable to simply remove it.

    \sa LH_EMPTY_SIGNATURE
*/
typedef struct lh_signature_t
{
    char marker[16]; /**< \sa LH_SIGNATURE_MARKER */
    char sign[256]; /**< 2048-bit RSA signature of the shared plugin's SHA-1 digest, PKCS1 padded */
    char url[128]; /**< URL to the public key */
    int size; /**< sizeof(lh_signature) */
} lh_signature;

/**
    Signature marker so SignPlugin can find it
    \sa lh_signature
*/
#define LH_SIGNATURE_MARKER {7,98,120,242,114,174,176,97,178,246,229,116,243,34,2,92}

/**
    Define a blank signature area
    \sa lh_signature
*/
#define LH_SIGNATURE_BLANK { LH_SIGNATURE_MARKER, {0}, {0}, sizeof(lh_signature) }
#define LH_SIGNATURE() lh_signature _lh_plugin_signature = { LH_SIGNATURE_MARKER, {0}, {0}, sizeof(lh_signature) }

typedef struct lh_buffer_t
{
    int n;
    const char *p;
} lh_buffer_t;

#ifdef __cplusplus
extern "C" {
#endif
/* Utility functions in lh_plugin.c */
void lh_buffer_t_to_headerfile( const lh_buffer_t *buffer, const char *filename, const char *varname );
lh_ui_type lh_name_to_userinterface( const char *name );
const char *lh_userinterface_to_name( const lh_ui_type ui );
#ifdef __cplusplus
}
#endif

#endif // LH_GLOBAL_H
