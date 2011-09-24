/**
  \file     lh_plugin.h
  \author   Johan Lindh <johan@linkdata.se>
  \brief    C API for LCDHost plugins
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
  These are the entry points for LCDHost plugins using the C API.
  See lh_qobject.h for the Qt C++ API.

  1.  If the plugin has a signature area, integrity is checked.
  2.  LCDHost examines the plugin's embedded XML document.
  3.  Either the user decides to load the plugin or it's set to auto load
  4.  LCDHost has the operating system load the shared library
  5.  \c lh_create() is called returning the plugin's \c lh_object*
  6.  \c obj_init() is called if it is provided by the plugin's \c lh_object*
    * at this point, barring errors, the plugin is considered loaded
    * when the user decides to unload the plugin or LCDHost shuts down:
  7.  \c lh_destroy() is called if it's provided by the plugin
  8.  LCDHost has the operating system unload the shared library

  lh_object *lh_create()
    Create the plugin object and return a lh_object pointer.
    LCDHost will provide this pointer when calling the functions
    in lh_object structure. Return NULL on error. The returned
    pointer must remain valid until lh_destroy() returns.

  void lh_destroy( lh_object *obj )
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

#ifndef LH_PLUGIN_H
#define LH_PLUGIN_H

#define LH_API_MAJOR 6
#define LH_API_MINOR 0
#define LH_DEVICE_MAXBUTTONS 32

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

#define LH_ALIGN_LEFT       0
#define LH_ALIGN_CENTER     1
#define LH_ALIGN_RIGHT      2
#define LH_ALIGN_TOP        LH_ALIGN_LEFT
#define LH_ALIGN_BOTTOM     LH_ALIGN_RIGHT
#define LH_ALIGN_MAX        LH_ALIGN_RIGHT

#define LH_METHOD_ADJUST    0   /**< add size offset size source */
#define LH_METHOD_PERMILLE  1   /**< make size X 1/1000:s of source (e.g. 1000 = same as source) */
#define LH_METHOD_LEFT      2   /**< adjust size so our right edge is the same as source left/top */
#define LH_METHOD_RIGHT     3   /**< adjust size so our right edge is the same as source right/bottom */
#define LH_METHOD_ABSOLUTE  4   /**< size is set to whatever offset is set to */
#define LH_METHOD_TOP       LH_METHOD_LEFT
#define LH_METHOD_BOTTOM    LH_METHOD_RIGHT
#define LH_METHOD_MAX       LH_METHOD_ABSOLUTE

#define NEXTBIT(v) (((v-1)>>1)|((v-1)>>2)|((v-1)>>4)|((v-1)>>8)|((v-1)>>16))

/**
    All plugins must embed an XML document containing build information.
    The document must be readable from the plugin shared library using
    standard binary file I/O. The document must contain the maintag
    <lcdhostplugin>. LCDHost will search the shared library for the
    string '<lcdhostplugin>' and then check to see that it's a correctly
    formed XML document. Encoding must be UTF-8. There must be only one
    copy of the string '<lcdhostplugin>' in the shared library.

    Note that the XML data needs to kept in the library. If
    it's not marked as an export, and not referenced anywhere,
    smart linkers are liable to simply remove it.

    Sample C code to embed the document, copy, paste and modify to suit.
    Note that you could also embed the document using other methods.

      char __lcdhostplugin_xml[] =
      "<?xml version=\"1.0\"?>"
      "<lcdhostplugin>"
        "<id>NAME</id>"
        "<rev>" STRINGIZE(REVISION) "</rev>"
        "<api>" STRINGIZE(LH_API_MAJOR) "." STRINGIZE(LH_API_MINOR) "</api>"
        "<ver>" "r" STRINGIZE(REVISION) "</ver>"
        "<versionurl>http://www.linkdata.se/lcdhost/version.php?arch=$ARCH</versionurl>"
        "<author>Johan \"SirReal\" Lindh</author>"
        "<homepageurl><a href=\"http://www.linkdata.se/software/lcdhost\">Link Data Stockholm</a></homepageurl>"
        "<logourl></logourl>"
        "<shortdesc>"
        "ONE_LINE_DESCRIPTION"
        "</shortdesc>"
        "<longdesc>"
        "MULTI_LINE_DESCRIPTION"
        "</longdesc>"
      "</lcdhostplugin>";

    'versionurl' is the version information URL. It replaces $ID to <id>,
    $ARCH to the current architecture and $REV to <rev>.
    This URL should return a text/xml document like the following sample:

    <lhver arch="win32" url="http://lcdhost.googlecode.com/files/$ID_$ARCH_R$REV.zip">
    <f id="LH_Text" r="6" />
    </lhver>

    The 'lhver' element contains default attribute values for 'f' elements.
    The 'url' attribute expands the same parameters as the version url.

    A 'f' element may contain the following attributes:
    'id'        The plain filename, without system prefix or suffixes
    'arch'      The architecture (ex, 'win32', 'mac32' or 'lin64d')
    'r'         The revision number
    'url'       The download URL
    'api'       The API versions in the form 'major.minor'

    There may be any number of 'f' elements.

    The document will be cached, so if several plugins refer to the same URL,
    the cached copy will be used. The cache is cleared intermittently.
  */

/**
  Utility macro to be able to use the value of numeric
  preprocessor macros as strings.
  \sa http://stackoverflow.com/questions/2751870/how-exactly-does-the-double-stringize-trick-work
  */
#ifndef STRINGIZE
# define STRINGIZE_(x) #x
# define STRINGIZE(x) STRINGIZE_(x)
#endif

/**
    Maximum lh_object.ident size, including nul terminator
    \sa lh_object
*/
#define LH_MAX_IDENT            64

/**
  Forward declarations of structs and typedefs.
  */
struct lh_object_t;
typedef struct lh_object_t lh_object;

struct lh_layout_item_t;
typedef struct lh_layout_item_t lh_layout_item;

/**
  Simple data buffer.
  */
typedef struct lh_buffer_t
{
    const char *p; /**< pointer to buffer data area */
    int n; /**< size of data stored in buffer data area */
} lh_buffer;

/**
  Select user interface for a lh_setup_item.
  */
typedef enum lh_userinterface_t
{
    lh_ui_none = 0,
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
    lh_ui_unused
} lh_userinterface;

typedef enum lh_meta_flag_t
{
    lh_save = (1<<0), /**< the property should be saved if modified (Qt STORED) */
    lh_sink = (1<<1), /**< the property may be used as a data sink (Qt WRITE) */
    lh_source = (1<<2), /**< the property may be used as a data source (Qt SCRIPTABLE) */
    lh_export = (1<<3), /**< the property is shown in the data link UI (Qt USER) */
    lh_flag_mask = NEXTBIT(lh_export)-1
} lh_meta_flag;

typedef enum lh_meta_format_t
{
    lh_format_none = lh_flag_mask+1,
    lh_format_integer, /**< integer, \c const \c int* */
    lh_format_longlong, /**< long long integer, \c const \c long \c long* */
    lh_format_boolean, /**< nonzero is true, zero is false, \c const \c int* */
    lh_format_color, /**< 32-bit AARRGGBB color data, \c const \c int* */
    lh_format_double, /**< double, \c const \c double* */
    lh_format_local8, /**< null terminated local 8-bit encoded string, \c const \c char* */
    lh_format_utf8, /**< UTF-8 encoded string, \c const \c lh_buffer* */
    lh_format_font, /**< UTF-8 encoded font data (using QFont::toString()), \c const \c lh_buffer* */
    lh_format_input, /**< lh_input, \c const \c lh_buffer* */
    lh_format_png, /**< PNG data, \c const \c lh_buffer* */
    lh_format_list, /**< array of lh_property, \c const \c lh_buffer* */
    lh_format_unused,
    lh_format_mask = NEXTBIT(lh_format_unused)-1
} lh_meta_format;

typedef enum lh_meta_role_t
{
    lh_role_none = lh_format_mask+1,
    lh_role_title, /**< object's title */
    lh_role_dir, /**< object's data directory, may not be writeable */
    lh_role_help, /**< object's help text, HTML */
    lh_role_order, /**< object's sort order value */
    lh_role_import, /**< object's data link import path, ASCII with forward slash separators */
    lh_role_export, /**< object's data link export path, ASCII with forward slash separators */
    lh_role_enabled, /**< object is enabled in UI (includes children) */
    lh_role_visible, /**< object is visible in UI (includes children) */
    lh_role_value, /**< primary data value */
    lh_role_minimum, /**< primary data value lower bound */
    lh_role_maximum, /**< primary data value upper bound */
    lh_role_list, /**< primary data value should be selected from this list */
    lh_role_user, /**< don't use the enum value directly, use the lh_cb_role callback */
    lh_role_mask = ~(lh_flag_mask|lh_format_mask)
} lh_meta_role;

/**
  Encodes LCDHost primitive property metadata.

  Encodes the property role, the data format (how to interpret the
  'const void*'), the saving policy (if LCDHost should try to save
  and restore the property between sessions), and the data linking
  policy (if the property data is available as a source, or may be
  used as a sink).

  The property role and data format must always be valid
  and respected by the receiver of a property update.
  Check the respective lh_format_xxx enum values for
  how to cast the \c const \c void* passed.

  LCDHost will perform property updates using the same data
  format that the object used, if possible.

  The saving and linking flags are always controlled by the
  plugin, and the plugin should ignore those flags when
  receiving property updates from LCDHost.
  */

typedef int lh_meta;
const lh_meta lh_meta_default = (lh_save|lh_sink|lh_source);

#define lh_prop_format(meta) ((lh_meta_format)(meta&lh_format_mask))
#define lh_prop_role(meta) ((lh_meta_role)(meta&lh_role_mask))

/**
  Struct keeping a metadata and data together.
  Used for lh_format_list.
  */
typedef struct lh_property_t
{
    lh_meta meta;
    union
    {
        int i;
        long long ll;
        double d;
        const char *p;
        lh_buffer b;
    } data;
} lh_property;

/**
  LCDHost has a number of objects that export their properties
  for use with data linking. They all export the lh_role_value
  property, along with lh_role_maximum and lh_role_minimum.

  Link path and suggested data format if lh_format_qvariant
  is not available to you:

  "/lcdhost/dir/data"       lh_format_utf8
  "/system/cpu/count"       lh_format_integer
  "/system/cpu/coreload"

  */


/**
  Used with the callback function.
  \sa lh_callback_t
  \sa lh_object.cb_fn
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

    /* sent from a lh_object, these create children of that object */
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
    char ident[LH_MAX_IDENT]; /**< device identity, \sa lh_object.ident */
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

/**
  The basic LCDHost plugin object structure. This is embedded in all more
  complex structures, but also returned by the exported function lh_create().
  */
struct lh_object_t
{
    /**
      sizeof(lh_object)

      The \c size member is used to version the lh_object structure.
      Always initialize it to sizeof(lh_object). When you receive
      a previously unseen pointer to an lh_object, it's recommended
      you check \c size and return an error message if it doesn't
      match.
      */
    int size;

    /**
      Plugin-to-LCDHost callback ID.

      This uniquely identifies the object to LCDHost. It must be
      initialized to zero by the plugin when it creates objects.
      It is set by LCDHost before calling \c obj_init(). It is
      always the first parameter in the plugin-to-LCDHost
      callbacks.
      */
    const void *cb_id;

    /**
      Plugin-to-LCDHost callback function.

      Initialize to zero. Will be set by LCDHost before
      \c obj_init() is called.

      \param cb_id  Callback ID.
      \param code   Callback code, \see lh_callbackcode
      \param param  Integer parameter, use depends on callback code
      \param data   Data pointer, use depends on callback code
      \return       Depends on callback code
      */
    lh_callback_t cb_fn;

    /**
      For plugin internal use.

      The \c ref member may be used by the plugin for it's own
      purposes. It's never interpreted or altered by LCDHost.
      */
    void *ref;

    /**
      Object identity string.

      The identity of an object is set by the creator of the object:
      LCDHost sets it for plugins and layout items.
      The plugin sets it for setup items, layout classes and devices.
      An identity string has several restrictions on it:
        It must be ASCIIZ (a null-terminated ASCII string).
        It must not be longer than LH_MAX_IDENT, including the null terminator.
        It should be unique among siblings.
        It must not contain any of the following characters:
            Forward slash '/'
            Backslash '\'
            Double quote '"'
            Less than '<'
            Greater than '>'
    */
    char ident[LH_MAX_IDENT];

    /*
      Functions exported by the plugin object to LCDHost. None of these
      are required. Any and all of them may be set to NULL if the plugin
      doesn't require the service they provide.
      */

    /**
      Called by LCDHost when the object is being initialized. The
      callback ID \c cb_id and callback function pointer \c cb_fn will
      have been set by LCDHost. May be set to NULL if not needed.

      \param obj    The lh_object being initialized.
      \return       NULL if the object initialized successfully, or a pointer to an ASCIIZ error text.
      */
    const char* (*obj_init)( lh_object* obj );

    /**
      Called by LCDHost to create or set a primitive property on the object.
      Update your internal property value and return.

      \param obj    The lh_object being modified.
      \param prop   New property data, \see lh_property
      */
    void (*obj_set_property)( lh_object* obj, const lh_property* prop );

    /**
      Polling service function.

      Called once after obj_init() to set the initial polling interval.
      Note that the polling interval is never exact. If you need precise
      timing, use a separate thread and operating system services.

      \param obj    The object.
      \return       Polling interval in milliseconds, or zero to not use polling.
      */
    int (*obj_polling)(lh_object* obj);

    /**
      Event notification function.

      Notifies the object of events that occur.
      \param obj    The object.
      \param event  \see LH_NOTE_xxx defines
      \param param  Use depends on \p event
      */
    void (*obj_event)( lh_object* obj, lh_eventcode event, const void* param );
};

/**
  Setup items are used to provide user interface
  elements for plugin objects.
  */
typedef struct lh_setup_item_t
{
    lh_object obj; /**< \sa lh_object */
    int size; /**< sizeof(lh_setup_item) */
    lh_userinterface ui; /**< user interface */
} lh_setup_item;

/**
  If the backlight of an output device is supported, this
  structure is may be used to control it.
  */
typedef struct lh_device_backlight_t
{
    int r, g, b; /* backlight color setting */
    char is_present; /* device has backlight */
    char is_readable; /* device backlight setting is readable */
    char is_writeable; /* device backlight setting is writable */
} lh_device_backlight;

/**
  This structure gives basic information about an output device.

  For devices, both output and input, it's required that the
  'obj.ident' string identifies the device uniquely globally,
  and not just among it's siblings. It is suggested to use
  the hexadecimal HID values in the format

    vendor:product:version:page:usage

  Two devices must not share the same lh_output_device
  instance.

  Since the lh_output_device is created by the plugin, it must also be destroyed
  by it. Notify LCDHost of destruction using the lh_cb_destroy callback.
  */
typedef struct lh_output_device_t
{
    lh_object obj; /**< \sa lh_object */
    int size; /**< sizeof(lh_output_device) */

    int width; /**< width in pixels */
    int height; /**< height in pixels */
    int depth; /**< bit depth */
    int noauto; /**< don't autoselect this device; manual selection only */

    /* functions */
    const char* (*obj_open)(struct lh_output_device_t*); /**< device is selected by user for output */
    const char* (*obj_render_qimage)(struct lh_output_device_t*,void*); /**< render a QImage onto the device */
    const char* (*obj_render_argb32)(struct lh_output_device_t*,int,int,const void*); /**< render an ARB32 bitmap onto the device */
    const char* (*obj_render_mono)(struct lh_output_device_t*,int,int,const void*); /**< render a monochrome one-byte-per-pixel map */
    const char* (*obj_get_backlight)(struct lh_output_device_t*,lh_device_backlight*); /**< return current backlight state */
    const char* (*obj_set_backlight)(struct lh_output_device_t*,lh_device_backlight*); /**< change current backlight state */
    const char* (*obj_close)(struct lh_output_device_t*); /**< device is no longer selected for output */
} lh_output_device;

/**
  This structure gives basic information about an input device.

  For devices, both output and input, it's required that the
  'obj.ident' string identifies the device uniquely globally,
  and not just among it's siblings. It is suggested to use
  the hexadecimal HID values in the format

    vendor:product:version:page:usage

  Two devices must not share the same lh_input_device
  instance. To provide input data from the device to
  LCDHost, use the lh_cb_input callback.

  Resolving an input device name or input device item's name
  may be an expensive operation. LCDHost will call obj_input_name
  to do so when required only, and will cache the results.
  Calling obj_input_name with a zero value for the item requests
  the device name, otherwise the device item name is requested.

  The obj_emulate function is used when a LCDHost macro
  wants to send a fake event using the device to the rest
  of the operating system.

  Since the lh_input_device is created by the plugin, it must also be destroyed
  by it. Notify LCDHost of destruction using the lh_cb_destroy callback.
  */
typedef struct lh_input_device_t
{
    lh_object obj; /**< \sa lh_object */
    int size; /**< sizeof(lh_input_device) */

    int flags; /**< \sa lh_input_flag */

    /* functions */
    const char* (*obj_open)(struct lh_input_device_t*); /**< user wants input events from this device */
    const char* (*obj_input_name)(struct lh_input_device_t*,int); /**< map input item to human readable name */
    void (*obj_emulate)(struct lh_input_device_t*,int,int,int); /**< emulate device input to rest of system */
    const char* (*obj_close)(struct lh_input_device_t*); /**< user no longer wants input events */
} lh_input_device;

/**
  While layout items in LCDHost are ordered in a hierarchy rooted
  at a layout, at the plugin level that ordering is hidden.

  The lh_layout_item lifetime is determined by LCDHost, and the
  plugin must not destroy it before obj_layout_item_destroy has
  been called for it. In addition, the plugin must not call
  lh_cb_destroy for layout items.

  LCDHost will set the 'dir' property of the layout item to
  be the directory of the layout it belongs to. Note that this
  directory may very well be read-only.
  */
struct lh_layout_item_t
{
    lh_object obj;
    int size; /**< sizeof(lh_layout_item) */

    /* functions */
    void (*obj_prerender)(lh_layout_item*,int); /**< called right before width/height/render_xxx as a notification, param: bitdepth */
    int (*obj_width)(lh_layout_item*,int); /**< return suggested width given a height (or -1 for default width) */
    int (*obj_height)(lh_layout_item*,int); /**< return suggested height given a width (or -1 for default height) */
    const lh_buffer* (*obj_render_buffer)(lh_layout_item*,int,int,int); /**< render object to any image format using width x height x bitdepth */
    void* (*obj_render_qimage)(lh_layout_item*,int,int,int); /**< render object to QImage using width x height x bitdepth */
};

/**
  This structure gives basic information about a layout class.

  Both the path and name members are UTF-8 encoded and NULL terminated strings.

  \c path is where to place the class in the class list tree. It must be in
  english, and LCDHost may attempt to translate the path items into the users
  language. Example: '/System/Date and time'

  \c obj.ident is the internal identifier for this class. It should remain static
  on all platforms/languages. The name of the plugin plus this will be used to
  identify the class uniquely.

  The width/height members are used to estimate the size of an instance before
  actually creating it. Set to -1 if you have no idea.

  Since the lh_layout_class is created by the plugin, it must also be destroyed
  by it. Notify LCDHost of destruction using the lh_cb_destroy callback.
  */
typedef struct lh_layout_class_t
{
    lh_object obj;

    int size; /* sizeof(lh_layout_class) */

    /* set by plugin */
    const char *path; /* UTF-8 encoded, must NOT be localized */
    int width;
    int height;

    /* functions */
    lh_layout_item* (*obj_layout_item_create)(struct lh_layout_class_t*); /**< create a new layout item */
    void (*obj_layout_item_destroy)(struct lh_layout_class_t*,lh_layout_item*); /**< destroy this layout item */
} lh_layout_class;

#ifdef __cplusplus
class lh_plugin_calltable
{
public:
    lh_object* (*lh_create)();
    void (*lh_destroy) (lh_object*);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Utility functions in lh_plugin.c */
void lh_buffer_to_headerfile( const lh_buffer *buffer, const char *filename, const char *varname );
lh_userinterface lh_name_to_userinterface( const char *name );
const char *lh_userinterface_to_name( const lh_userinterface ui );
lh_meta lh_name_to_data_format( const char *name );
const char *lh_data_format_to_name( const lh_meta );

#ifdef __cplusplus
}
#endif


#endif /* LH_PLUGIN_H */
