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

  1.  LCDHost examines the plugin file for build info XML and signature.
  2.  Either the user decides to load the plugin or it's set to auto load
  3.  LCDHost has the operating system load the shared library
  4.  lh_create() is called, requires non-NULL lh_object* return
      The pointer must remain valid until lh_destroy() returns.
  5.  obj_init() is called, returns NULL or error message
    * at this point, barring errors, the plugin is considered loaded
    * when the user decides to unload the plugin or LCDHost shuts down:
  6.  lh_destroy() is called
  7.  LCDHost has the operating system unload the shared library

  lh_object *lh_create( lh_callback_t, void* )
    Create the plugin object and return a lh_object pointer.
    LCDHost will provide this pointer when calling the functions
    in lh_object structure. Return NULL on error.

  void lh_destroy( lh_object *obj )
    Free resources associated with 'obj'. The shared library is about to be
    removed from memory.

  */

#ifndef LH_PLUGIN_H
#define LH_PLUGIN_H

#include <stddef.h>
#include <limits.h>

/* 64 bits integers for the common platforms */
/* Consider including <QtGlobal> for the real deal */
#if !defined(Q_INT64_C)
# if defined(_MSC_VER) && defined(_UI64_MAX)
#  define Q_INT64_C(c) c ## i64    /* signed 64 bit constant */
#  define Q_UINT64_C(c) c ## ui64   /* unsigned 64 bit constant */
typedef __int64 qint64;            /* 64 bit signed */
typedef unsigned __int64 quint64;  /* 64 bit unsigned */
# else
#  define Q_INT64_C(c) static_cast<long long>(c ## LL)     /* signed 64 bit constant */
#  define Q_UINT64_C(c) static_cast<unsigned long long>(c ## ULL) /* unsigned 64 bit constant */
typedef long long qint64;           /* 64 bit signed */
typedef unsigned long long quint64; /* 64 bit unsigned */
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
  Add new codes AT THE END, or existing plugins will break.
  */
typedef enum lh_callbackcode_t
{
    /* does not require a callback id */
    lh_cb_utf8_to_local8bit, /* request UTF-8 to local 8-bit conversion, param: char *string */
    lh_cb_load_layout, /* request loading of a new layout, param: UTF-8 encoded file name relative to data path */
    lh_cb_dir_binaries, /* get the a pointer to LCDHost binaries path in UTF-8, param: const char ** */
    lh_cb_dir_plugins, /* get the a pointer to LCDHost plugins path in UTF-8, param: const char ** */
    lh_cb_dir_data, /* get the a pointer to LCDHost data path in UTF-8, param: const char ** */

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
    lh_cb_setup_refresh_meta, /* LCDHost will re-read your setup item's metadata */
    lh_cb_setup_refresh_data, /* LCDHost will re-read your setup item's data */

    /* sent from lh_layout_item only */
    lh_cb_render, /* ask for a rendering sequence (prerender/width/height/render), param: NULL */
    lh_cb_sethidden, /* set visibility state for self and children, param int* */

    lh_cb_unused
} lh_callbackcode;

/**
  The plugin-to-LCDHost callback.
  */
typedef void (*lh_callback_t)( void* cb_id, lh_callbackcode code, void *param );

/**
    Definition of signature area
    The signature area is optional by highly recommended.
*/

typedef struct lh_signature_t
{
    char marker[16]; /* unique series to allow finding the sig */
    char sign[256]; /* 2048-bit RSA signature of the shared plugin's SHA-1 digest, PKCS1 padded */
    char url[128]; /* URL to the public key */
    int size; /* sizeof(lh_signature) */
} lh_signature;

#define LH_SIGNATURE_MARKER {7,98,120,242,114,174,176,97,178,246,229,116,243,34,2,92}

/* Declare a signature area - don't mess with the constants, */
/* they're there so that SignPlugin can find the right spot. */
#define LH_SIGNATURE() lh_signature _lh_plugin_signature = { LH_SIGNATURE_MARKER, {0}, {0}, sizeof(lh_signature) }

typedef enum lh_setup_type_t
{
    lh_type_none = 0x0000,

    lh_type_integer = 0x0100, /* integer selection using spinbox */
    lh_type_integer_boolean, /* checkbox */
    lh_type_integer_color, /* 32-bit AARRGGBB */
    lh_type_integer_slider, /* slider to select an integer */
    lh_type_integer_progress, /* progress bar */

    lh_type_double = 0x0200, /* double selection using spinbox */

    lh_type_string = 0x0400, /* all strings are null-terminated, utf-8 encoded */
    lh_type_string_script, /* multiline text editor */
    lh_type_string_filename, /* present the user with an file selection dialog */
    lh_type_string_font, /* simple font selection, see QFont::toString() for string format */
    lh_type_string_inputstate, /* ask user for a specific button state or half axis movement */
    lh_type_string_inputvalue, /* ask user for a button or full axis movement */
    lh_type_string_button, /* a clickable button */
    lh_type_string_htmllink, /* receive clicked links from the title */
    lh_type_string_list, /* using dropdown box, have user select one of the param.list strings */
    lh_type_string_listbox, /* using listbox, have user select one of the param.list strings */
    lh_type_string_combobox, /* using combobox, allow user to select or type a string value */

    lh_type_pointer = 0x0800, /* not saved - store a pointer */
    lh_type_pointer_qimage, /* not saved - allows the display of a QImage in the setup pane, param.p */

    lh_type_array = 0x1000, /* basic fixed-size data storage */
    lh_type_array_png, /* allows the display of a PNG image */
    lh_type_array_qint64 = (lh_type_array|lh_type_integer), /* no UI - store an array of qint64 in buffer */
    lh_type_array_double = (lh_type_array|lh_type_double), /* no UI - store an array of doubles in buffer */
    lh_type_array_string, /* no UI - data is a list of NUL-delimited UTF-8 strings */

    lh_type_last /* marks last used value */
} lh_setup_type;

#define LH_FLAG_READONLY    0x0001 /* Setup item is disabled in GUI */
#define LH_FLAG_HIDDEN      0x0002 /* Setup item is excluded from GUI */
#define LH_FLAG_FOCUS       0x0004 /* Setup item is focused when user double clicks instance */
#define LH_FLAG_AUTORENDER  0x0008 /* Request a render when changed */
#define LH_FLAG_FIRST       0x0010 /* Used with LH_QtSetupItem's - Same as calling setOrder(-1) */
#define LH_FLAG_LAST        0x0020 /* Used with LH_QtSetupItem's - Same as calling setOrder(1) */
#define LH_FLAG_NOSAVE      0x0040 /* Setup item won't be saved to or loaded from layout */
#define LH_FLAG_BLANKTITLE  0x0080 /* Setup item title is not shown in GUI (blank space is shown) */
#define LH_FLAG_NOSOURCE    0x0100 /* Setup item must not be used as a data source */
#define LH_FLAG_NOSINK      0x0200 /* Setup item must not be used as a data sink */
#define LH_FLAG_HIDETITLE   0x0400 /* Setup item title is not shown in GUI (all space to value) */
#define LH_FLAG_HIDEVALUE   0x0800 /* Setup item value is not shown in GUI (all space to title) */
#define LH_FLAG_MIN         0x1000 /* Limit UI numeric input to the min value given in params */
#define LH_FLAG_MAX         0x2000 /* Limit UI numeric input to the max value given in params */
#define LH_FLAG_MINMAX      0x3000 /* Limit UI numeric input to the min and max value given in params */

#define LH_STATE_SOURCE     0x0001 /* Setup item is a data source */

#define LH_MAX_IDENT        64     /* Maximum lh_object identifier length, including nul terminator */

/**
  Stores 'secondary' data for setup items. These are not
  preserved by LCDHost, but are used to control UI elements.
  The min/max values limit UI input only; programmatically
  you may set any value. When using data linking, parameters
  will be copied from a data source to a data sink. This is
  useful since having min and max values help making rendering
  decisions.
  */
typedef union lh_setup_param_t
{
    struct {
        qint64 min;
        qint64 max;
    } i;
    struct {
        double min;
        double max;
    } d;
    const char *list; /**< newline-delimited list of UTF-8 strings */
    void *p; /* for lh_type_image_qimage, points to a QImage */
} lh_setup_param;

/**
  Stores 'primary' data for setup items. This is the
  data that will be preserved by LCDHost.
  */
typedef union lh_setup_data_t
{
    struct {
        size_t n; /* buffer size */
        void *p; /* buffer pointer */
    } b;
    double d; /* floating point data */
    qint64 i; /* integer data */
} lh_setup_data;

/**
  The basic LCDHost plugin object structure. This is embedded in all more
  complex structures, but also returned by the exported function lh_create().
  */
typedef struct lh_object_t
{
    int size; /* sizeof(lh_object) */
    void *ref; /* internal plugin reference, never interpreted or altered by LCDHost */

    /* these are set by LCDHost, initialize to zero */
    void *cb_id; /* callback ID */
    lh_callback_t cb; /* callback function */

    /* functions */
    const char* (*obj_init)(struct lh_object_t*); /**< return error msg or NULL */
    int (*obj_polling)(struct lh_object_t*); /**< return ms to wait before next call, or zero to stop polling */
    int (*obj_notify)(struct lh_object_t*,int,void*); /**< return wanted notification mask, see LH_NOTE_xxx */

    /* the identity of an object is set by LCDHost for plugins and layout items */
    /* for setup items, layout classes and devices, it is set by the plugin. */
    /* an ident may not contain any of the following characters: '/', '\', '"' '<', '>' */
    char ident[LH_MAX_IDENT]; /* ASCIIZ, must be unique among siblings */

    /* the title of an object may be any UTF-8 string. the pointer may be NULL. */
    /* the title is set and changed by the party that control the object lifetime. */
    /* for plugins and layout items, this is LCDHost. for layout classes, setup items */
    /* and devices, this is the plugin. use the lh_cb_title_refresh callback when needed. */
    const char *title; /* UTF-8 encoded title */
} lh_object;

/**

    Setup items are the main information link between LCDHost and it's plugins.
    They're identified by 'obj.ident', which must be an ASCII string
    containing no forward slashes. It is case sensitive.

    LCDHost will preserve the data portion of a setup item, and setup items
    will be set to the stored data between sessions unless the LH_FLAG_NOSAVE
    flag is set. All other parts of the setup item are dictated by the plugin.

    Setup items can be set to export their own value or import another setup
    items value. This is known as 'data linking'. A setup item may be either
    a 'data source' exporting it's value or a 'data sink' importing another
    item's value.

    If the state LH_STATE_SOURCE is set, then the item will export it's value.
    By default, it's exported using the ID, but the 'link' member may be set
    to export it under any valid path.

    If the LH_STATE_SOURCE is NOT set, and the 'link' member is not NULL, then
    the item is a data sink, and will receive updates from data sources with
    the same 'link'.

    The LH_FLAG_NOSINK and LH_FLAG_NOSOURCE flags override the LH_STATE_SOURCE
    state and link path, and if set, will prevent the data item from either receiving
    or exposing data, respectively.

    In addition, the 'filter' member causes the LCDHost UI to only show sources
    and sinks with the same filter.

    Since the lh_setup_item is created by the plugin, it must also be destroyed
    by it. Notify LCDHost of destruction using the lh_cb_destroy callback.
*/
typedef struct lh_setup_item_t
{
    lh_object obj; /* basic object, see above */

    int size; /* sizeof(lh_setup_item) */

    /* the metadata portion, controlled by plugin */
    const char *help; /* short HTML help text shows as tooltip or as value, may be NULL */
    const char *filter; /* data link filter, NULL if unused */
    int order; /* ordering of setup item in the UI, lower values first */
    lh_setup_type type; /* type of data, see enum above */
    int flags; /* LH_FLAG_xxx */
    lh_setup_param param; /* UI parameters, list data etc */

    /* the data portion, these things are saved by LCDHost */
    unsigned states; /* LH_STATE_xxx */
    const char *link; /* data link path, see notes above, usually NULL */
    lh_setup_data data;

    /* functions */
    void (*obj_setup_resize)(struct lh_setup_item_t*,size_t); /**< data storage is too small, please resize */
    void (*obj_setup_change)(struct lh_setup_item_t*); /**< data has been changed by LCDHost */
} lh_setup_item;

/**
  */
typedef struct lh_device_backlight_t
{
    int r, g, b; /* backlight color setting */
    char is_present; /* device has backlight */
    char is_readable; /* device backlight setting is readable */
    char is_writeable; /* device backlight setting is writable */
} lh_device_backlight;

/**
  The device flags are used to describe an input device
  and input device event in general.
  */

enum lh_device_flag
{
    lh_df_button      = 0x0001,
    lh_df_value       = 0x0002,
    lh_df_down        = 0x0004, // button down or positive values
    lh_df_pos         = 0x0004,
    lh_df_up          = 0x0008, // button up or negative values
    lh_df_neg         = 0x0008,
    lh_df_keyboard    = 0x1000,
    lh_df_mouse       = 0x2000,
    lh_df_joystick    = 0x4000,
    lh_df_other       = 0x8000,

    lh_df_state       = 0x0FFF,
    lh_df_device      = 0xF000
};

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
    lh_object obj;

    int size; /* sizeof(lh_output_device) */
    int width; /* width in pixels */
    int height; /* height in pixels */
    int depth; /* bit depth */
    int noauto; /* don't autoselect this device; manual selection only */

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
    char ident[LH_MAX_IDENT]; /* device identity, same as lh_input_device.ident */
    int item; /* control item identifier, must not be zero */
    int flags; /* describes kind of control and basic state, see lh_device_flag */
    int value; /* the exact value of the control */
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
    lh_object obj;

    int size; /* sizeof(lh_input_device) */
    int flags; /* lh_device_flag describing the device */

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
    lh_layout_item* (*obj_layout_item_create)(struct lh_layout_class_t*,lh_callback_t,void*); /**< create a new layout item */
    void (*obj_layout_item_destroy)(struct lh_layout_class_t*,lh_layout_item*); /**< destroy this layout item */
} lh_layout_class;

#ifdef __cplusplus
class lh_plugin_calltable
{
public:
    lh_object* (*lh_create)( lh_callback_t, void* );
    void (*lh_destroy) (lh_object*);
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Utility functions in lh_plugin.c */
lh_blob *lh_binaryfile_to_blob( const char *filename ); /* Caller must free() the blob */
void lh_blob_to_headerfile( lh_blob *blob, const char *filename, const char *varname );

#ifdef __cplusplus
}
#endif


#endif /* LH_PLUGIN_H */
