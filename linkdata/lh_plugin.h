/**
  \file     lh_plugin.h
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

/**
  These are the entry points for LCDHost plugins. Normal sequence is:

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

  */

#ifndef LH_PLUGIN_H
#define LH_PLUGIN_H

#ifdef __cplusplus
# ifdef QT_CORE_LIB
#  include <QVariant>
   typedef struct lh_variant_t lh_variant;
   void qVariantSetValue( QVariant&, const lh_variant& );
   bool operator>>( const QVariant&, lh_variant& lhv );
# endif
#endif

#define LH_API_MAJOR 6
#define LH_API_MINOR 0
#define LH_DEVICE_MAXBUTTONS 32

/**
  Notify flags. One or more of these are set in the 'note' parameter to the
  notify() function, and used in the return value from that same function
  to set the notification mask. Return zero if you're not interested in notifications.
  */
#define LH_NOTE_NONE        0x00000000
#define LH_NOTE_SECOND      0x00000001 /* Once-a-second notification */
#define LH_NOTE_CPU         0x00000002 /* CPU load data updated */
#define LH_NOTE_MEM         0x00000004 /* memory load data updated */
#define LH_NOTE_NET         0x00000008 /* network load data updated */
#define LH_NOTE_DEVICE      0x00000010 /* output device has changed */
#define LH_NOTE_WARNING     0x00000020 /* warnings, param: LH_WARNING_xxx (cast param to int) */
#define LH_NOTE_TITLE       0x00000040 /* title has been changed, param is NULL, obj.title ptr changed */
#define LH_NOTE_INPUT       0x00000080 /* input to a lh_type_input_xxx setup item, param: lh_input* */
#define LH_NOTE_INITIALIZED 0x00000100 /* object and all it's children have completed obj_init() ok */

#define LH_WARNING_DUPLICATE_SOURCE 1

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
  The lh_blob type is used to embed binary data, usually a JPG or PNG image file.
  */
typedef struct lh_blob_t
{
    unsigned len; /* sizeof(data) */
    unsigned sign; /* 0xDEADBEEF */
    unsigned char data[1]; /* actual data, length varies */
} lh_blob;

/**
  Used with lh_callback().
  */
typedef enum lh_callbackcode_t
{
    /* does not require a callback id */
    lh_cb_utf8_to_local8bit, /* request UTF-8 to local 8-bit conversion, param: char *string */
    lh_cb_load_layout, /* request loading of a new layout, param: UTF-8 encoded file name relative to data path */
    lh_cb_dir_binaries, /* get the pointer to LCDHost binaries path in UTF-8, param: const char ** */
    lh_cb_dir_plugins, /* get the pointer to LCDHost plugins path in UTF-8, param: const char ** */
    lh_cb_dir_data, /* get the pointer to LCDHost data path in UTF-8, param: const char ** */
    lh_cb_data_to_qvariant, /* Convert a lh_setup_data.value into a QVariant, param: lh_data_to_qvariant* */
    lh_cb_qvariant_to_data, /* Convert a QVariant into a lh_setup_data.value, param: lh_qvariant_to_data* */

    /* sent from anything */
    lh_cb_unload, /* ask that the plugin be unloaded, param: NULL or const char *message */
    lh_cb_reload, /* request the plugin reloaded, param: NULL or const char *message */
    lh_cb_log, /* add an UTF-8 encoded HTML string in the LCDHost log */
    lh_cb_polling, /* ask for a call to the polling function, param: NULL */
    lh_cb_notify, /* ask for a call to the notify function, param: NULL */
    lh_cb_title_refresh, /* object title has been changed, param: const char *newtitle */

    /* sent from a lh_object, these create children of that object */
    lh_cb_setup_create, /* create a new setup item, param: lh_setup_item* */
    lh_cb_output_create, /* a new output device have been detected, param: lh_output_device* */
    lh_cb_input_create, /* a new input device have been detected, param: lh_input_device* */
    lh_cb_class_create, /* a new layout class is available, param: lh_layout_class* */

    /* may be sent from any object, but usually sent from input devices */
    lh_cb_input, /* an input device state has changed, param is pointer to lh_input */

    /* these remove a plugin created item */
    lh_cb_destroy, /* destroy a setup item, layout class, or device */

    /* sent from lh_setup_item */
    lh_cb_setup_refresh_meta, /* LCDHost will re-read your setup item's flags, order and help */
    lh_cb_setup_refresh_link, /* LCDHost will re-read your setup item's link */
    lh_cb_setup_refresh_param, /* LCDHost will re-read your setup item's param */
    lh_cb_setup_refresh_value, /* LCDHost will re-read your setup item's data */

    /* sent from lh_layout_item */
    lh_cb_render, /* ask for a rendering sequence (prerender/width/height/render), param: NULL */
    lh_cb_sethidden, /* set visibility state for self and children, param int* */

    lh_cb_unused
} lh_callbackcode;

/**
  These flags are used to indicate how \c lh_variant is
  formatted and which members of contain valid data.
  The least significant four bits are reserved for UI
  selection, and are ignored by \c lh_variant handling.
  */
typedef enum lh_format_t
{
    lh_format_none      = 0,

    /* which member of \c lh_variant is valid */
    lh_store_integer    = 0x0010, /**< \c long \c long value stored in \c lh_variant.i */
    lh_store_double     = 0x0020, /**< \c double value stored in \c lh_variant.d */
    lh_store_pointer    = 0x0040, /**< \c void* stored in \c lh_variant.p */
    lh_store_buffer     = 0x0080, /**< \c lh_buffer* stored in \c lh_variant.b */

    /* data format enumeration */
    lh_format_boolean   = 0x0100|lh_store_integer,  /**< \c bool */
    lh_format_color     = 0x0200|lh_store_integer,  /**< \c AARRGGBB color value */
    lh_format_integer   = 0x0300|lh_store_integer,  /**< \c long \c long */
    lh_format_double    = 0x0400|lh_store_double,   /**< \c double */
    lh_format_pointer   = 0x0500|lh_store_pointer,  /**< \c void* */
    lh_format_qimage    = 0x0600|lh_store_pointer,  /**< \c QImage* */
    lh_format_buffer    = 0x0700|lh_store_buffer,   /**< \c char buffer */
    lh_format_string    = 0x0800|lh_store_buffer,   /**< UTF-8 encoded, NUL terminated string data */
    lh_format_input     = 0x0900|lh_store_buffer,   /**< \c lh_input */
    lh_format_png       = 0x0A00|lh_store_buffer,   /**< \c PNG data */
    lh_format_font      = 0x0B00|lh_store_buffer,   /**< \c QFont::toString() data, ASCII only */
    lh_format_array     = 0x0C00|lh_store_buffer,   /**< \c lh_variant array */

    lh_format_unused
} lh_format;

/**
  These are the setup item types allowed. These dictacte both the kind of UI
  that should be used and the underlying data type, as well as possible
  parameter usage.
  \sa lh_data_flag
  */
typedef enum lh_meta_type_t
{
    lh_type_none = 0,

    lh_type_boolean = lh_format_boolean+1, /**< checkbox */
    lh_type_color = lh_format_color+1, /**< line editor with color picker */
    lh_type_font = lh_format_font+1, /**< line editor with font picker */
    lh_type_png = lh_format_png+1, /**< decode PNG data and show the image */
    lh_type_integer = lh_format_integer+1, /**< line editor with spinbox (optional lh_format_pair: min/max) */
    lh_type_integer_slider, /**< slider (requires min/max) */
    lh_type_integer_progress, /**< progress bar (optional min/max) */
    lh_type_double = lh_format_double+1, /**< line editor with spinbox (optional min/max) */
    lh_type_string = lh_format_string+1, /**< line editor */
    lh_type_string_script, /**< multiline text editor */
    lh_type_string_filename, /**< line editor and file selection dialog (optional lh_type_string: file selection mask) */
    lh_type_string_button, /**< a button (required lh_type_string: button text) */
    lh_type_string_htmllink, /**< readonly clicked link text text (required lh_type_string: html to show) */
    lh_type_string_dropdownbox, /**< dropdown box (required lh_type_array: selectable values) */
    lh_type_string_listbox, /**< listbox (required lh_type_array: selectable values) */
    lh_type_string_combobox, /**< combobox (optional lh_type_array: selectable values) */
    lh_type_pointer = lh_format_pointer+1, /**< no UI - never saved */
    lh_type_pointer_qimage = lh_format_qimage+1, /**< shows the \c QImage - never saved */
    lh_type_input_value = lh_format_input+1, /**< readonly text and input selection dialog for button or axis */
    lh_type_input_state, /**< readonly text and input selection dialog for button state or half-axis */
    lh_type_buffer = lh_format_buffer+1, /**< no UI - general use data buffer */
    lh_type_array = lh_format_array+1, /**< no UI - array of lh_variant */

    lh_meta_type_unused /**< marks last used value */
} lh_meta_type;

/**
  Stores flags for \c lh_setup_meta.
  */
typedef enum lh_meta_flag_t
{
    lh_meta_none        = 0,

    lh_meta_save_value  = (1<<0), /**< LCDHost stores value between sessions */
    lh_meta_save_param  = (1<<1), /**< LCDHost stores params between sessions */
    lh_meta_save_link   = (1<<2), /**< LCDHost stores link between sessions */
    lh_meta_save = (lh_meta_save_value|lh_meta_save_param|lh_meta_save_link),

    lh_meta_show_link   = (1<<3), /**< data link button is shown in UI */
    lh_meta_show_help   = (1<<4), /**< help icon is shown in UI */
    lh_meta_show_title  = (1<<5), /**< title is shown in UI */
    lh_meta_show_value  = (1<<6), /**< value is shown in UI */
    lh_meta_show = (lh_meta_show_link|lh_meta_show_help|lh_meta_show_title|lh_meta_show_value),

    lh_meta_autorender  = (1<<7), /**< when changed, send parent re-render request */
    lh_meta_first       = (1<<8), /**< same as setting order to -1 */
    lh_meta_last        = (1<<9), /**< same as setting order to 1 */

    lh_meta_source      = (1<<10), /**< may be used as a data source */
    lh_meta_sink        = (1<<11), /**< may be used as a data sink */

    lh_meta_focus       = (1<<12), /**< wants initial UI focus when parent is selected */
    lh_meta_indent      = (1<<13), /**< indent title or data to match siblings */
    lh_meta_enabled     = (1<<14), /**< enabled in UI */
    lh_meta_visible     = (1<<15), /**< shown in UI */

    /* lh_meta_default is the same as having no flags in A18 */
    lh_meta_default = (lh_meta_save|lh_meta_show|lh_meta_source|lh_meta_sink|lh_meta_enabled|lh_meta_visible),

    lh_meta_flag_unused = (1<<16)  /**< first unused flag */
} lh_meta_flag;

/**
  The plugin-to-LCDHost callback.
  */
typedef void (*lh_callback_t)( void* cb_id, lh_callbackcode code, void *param );

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
    Maximum lh_object.ident size, including nul terminator
    \sa lh_object
*/
#define LH_MAX_IDENT            64

/**
  A plugin-managed memory buffer. If it needs to be resized,
  LCDHost may call the owner's obj_realloc() function.
  */
typedef struct lh_buffer_t
{
    char *p; /**< pointer to buffer data area */
    int m; /**< size of buffer data area */
    int n; /**< current size of data stored in buffer data area */
} lh_buffer;

/**
  The basic LCDHost plugin object structure. This is embedded in all more
  complex structures, but also returned by the exported function lh_create().
  */
typedef struct lh_object_t
{
    int size; /**< sizeof(lh_object) */
    void *ref; /**< internal plugin reference, never interpreted or altered by LCDHost */

    /* these are set by LCDHost, must be initialized to zero */
    void *cb_id; /**< callback ID, must be initialized to zero */
    lh_callback_t cb; /**< callback function, must be initialized to zero */

    /* functions */
    const char* (*obj_init)(struct lh_object_t*); /**< return error msg or NULL */
    int (*obj_polling)(struct lh_object_t*); /**< return ms to wait before next call, or zero to stop polling */
    int (*obj_notify)(struct lh_object_t*,int,void*); /**< return wanted notification mask, see LH_NOTE_xxx */
    void (*obj_realloc)(struct lh_object_t*,lh_buffer*,int); /**< request to resize memory buffer (set 'm', change 'p' if needed) */

    /* the identity of an object is set by LCDHost for plugins and layout items */
    /* for setup items, layout classes and devices, it is set by the plugin. */
    /* an ident may not contain any of the following characters: '/', '\', '"' '<', '>' */
    char ident[LH_MAX_IDENT]; /**< ASCIIZ, must be unique among siblings */

    /* the title of an object may be any UTF-8 string. the pointer may be NULL. */
    /* the title is set and changed by the party that control the object lifetime. */
    /* for plugins and layout items, this is LCDHost. for layout classes, setup items */
    /* and devices, this is the plugin. use the lh_cb_title_refresh callback when needed. */
    const char *title; /**< UTF-8 encoded title, NUL terminated */

} lh_object;

/**
  Stores metadata about the setup item. This is never saved by LCDHost,
  and must be provided by the plugin on creation of the \c lh_setup_item.
  */
typedef struct lh_setup_meta_t
{
    lh_meta_type type; /**< setup item type, selects UI and basic data type \sa lh_meta_type */
    int flags; /**< usually set to \c lh_meta_default \sa lh_meta_flag */
    int order; /**< ordering of setup item in the UI, lower values first */
    const char *help; /**< short UTF-8 HTML help text shown as icon with tooltip, may be NULL */
} lh_setup_meta;

/**
  Stores variable data for lh_setup_item.value and lh_setup_param.min/max/other
  \sa lh_format
  */
typedef struct lh_variant_t
{
    lh_format fmt; /**< \sa lh_format */
    union
    {
        long long i; /**< \sa lh_store_integer */
        double d; /**< \sa lh_store_double */
        void *p; /**< \sa lh_store_pointer */
        lh_buffer b; /**< \sa lh_store_buffer */
    } data;
#ifdef __cplusplus
# ifdef QT_CORE_LIB
    operator QVariant() const
    {
        QVariant v;
        qVariantSetValue(v,*this);
        return v;
    }
    lh_variant_t& operator =( const QVariant& v )
    {
        v >> *this;
        return *this;
    }
# endif
#endif
} lh_variant;

/**
  Parameter data is used to control UI elements or to provide
  rendering hints. They are not enforced by LCDHost, but UI elements
  will respect them where applicable.

  Parameters will be saved and restored if \c lh_meta_save_param is set.
  */
typedef struct lh_setup_param_t
{
    lh_variant min;
    lh_variant max;
    lh_variant other;
} lh_setup_param;

/**
  Stores flags for \c lh_setup_link.
  */
typedef enum lh_link_flag_t
{
    lh_link_source      = 0x0001,   /**< link is to be used as a source */
    lh_link_flag_unused = 0x8000    /**< first unused flag */
} lh_link_flag;

/**
  Setup items may be linked together to automatically transfer
  data changes between them. A setup item may act as a data source,
  a data sink, or neither. In addition, only setup items whose
  \c filter match may be linked. Creating multiple data
  sources with the same link name is allowed, but will cause
  LH_WARNING_DUPLICATE_SOURCE to be issued using obj_notify().
  */
typedef struct lh_setup_link_t
{
    const char *path; /**< ASCIIZ, link path, or NULL if neither source nor sink */
    int flags; /**< \sa lh_link_flag */
    const char *filter; /**< ASCIIZ, usually NULL */
} lh_setup_link;

/**
    Setup items are the main information storage for LCDHost plugins,
    and usual way LCDHost and the plugins exchange data.

    Note that all members of \c lh_setup_meta must be set, and that
    \c lh_setup_meta.type must not change during the lifetime of the \c lh_setup_item.

    Since the lh_setup_item is created by the plugin, it must also be destroyed
    by it. Notify LCDHost of destruction using the lh_cb_destroy callback.
  */
typedef struct lh_setup_item_t
{
    lh_object obj; /**< \sa lh_object */
    int size; /**< sizeof(lh_setup_item) */

    lh_setup_meta meta;
    lh_setup_param param; /**< parameters \sa lh_setup_param */
    lh_variant value; /**< value \sa lh_variant */
    lh_setup_link link;

    /* plugin exported functions, may be NULL */
    void (*obj_param_changed)(struct lh_setup_item_t*); /**< \c data.param has been changed by LCDHost */
    void (*obj_value_changed)(struct lh_setup_item_t*); /**< \c data.value has been changed by LCDHost */
    void (*obj_link_changed)(struct lh_setup_item_t*); /**< \c link has been changed by LCDHost */
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
  All layout items belong to a layout. Note that the values
  here may change, for instance if the layout is saved under
  a new name, saved in a new location or resized. The 'depth'
  member (bit depth) is tied to the device the layout is
  being displayed on, and defaults to 0 if the layout is
  not currently being displayed on a device.
  */
typedef struct lh_layout_t
{
    const char *dir; /* UTF-8 encoded directory, forward slash separators, ends in forward slash */
    const char *name; /* UTF-8 encoded file name, usually ends in .xml */
    int width;
    int height;
    int depth;
} lh_layout;

/**
  While layout items in LCDHost are ordered in a hierarchy rooted
  at a layout, at the plugin level that ordering is hidden.

  The lh_layout_item lifetime is determined by LCDHost, and the
  plugin must not destroy it before obj_layout_item_destroy has
  been called for it. In addition, the plugin must not call
  lh_cb_destroy for layout items.
  */
typedef struct lh_layout_item_t
{
    lh_object obj;

    int size; /* sizeof(lh_layout_item) */

    /* set by LCDHost, initialize to zero */
    lh_layout layout; /* layout info */

    /* functions */
    void (*obj_prerender)(struct lh_layout_item_t*); /**< called right before width/height/render_xxx as a notification */
    int (*obj_width)(struct lh_layout_item_t*,int); /**< return suggested width given a height (or -1 for default width) */
    int (*obj_height)(struct lh_layout_item_t*,int); /**< return suggested height given a width (or -1 for default height) */
    const lh_blob* (*obj_render_blob)(struct lh_layout_item_t*,int,int); /**< render object to any image format using width x height */
    void* (*obj_render_qimage)(struct lh_layout_item_t*,int,int); /**< render object to QImage using width x height */
} lh_layout_item;

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
lh_blob *lh_binaryfile_to_blob( const char *filename ); /* Caller must free() the blob */
void lh_blob_to_headerfile( lh_blob *blob, const char *filename, const char *varname );
lh_meta_type lh_name_to_meta_type( const char *name );
const char *lh_meta_type_to_name( const lh_meta_type t );
lh_format lh_name_to_format(const char *name );
const char *lh_format_to_name( const lh_format fmt );

#ifdef __cplusplus
}
#endif


#endif /* LH_PLUGIN_H */
