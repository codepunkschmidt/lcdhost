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
  These are the entry points for LCDHost libraries. As of LCDHost alpha 8, we don't
  separate plugins (which provide layout classes) from drivers (which provide
  output devices). They're both just libraries. For historical and portability
  reasons, they still reside in the 'plugins/' directory though.

  NOTE: All strings must be UTF-8 encoded unless explicitly stated otherwise.

  const char *lh_name()
    Return the name of the library.

  const char *lh_shortdesc()
    Return a short description of the library.

  int lh_version( int lcdhost_library_version )
    Parameter is the value of LCDHost's LH_LIBRARY_VERSION.
    Return your own LH_LIBRARY_VERSION.

  const char *lh_author()
    Return the author. You can use hyperlinks here, if you like.

  const char *lh_homepage()
    Return a hyperlink to the library's homepage, for example:
    "<a href=\"http://www.linkdata.se/software/lcdhost/\">Link Data Stockholm</a>"

  const char *lh_longdesc()
    Return a longer description of the library. Basic HTML and hyperlinks are OK.
    If you want an easy way to include rudimentary documentation about the library,
    this is the place to do it.

  const lh_blob *lh_logo()
    Return a lh_blob structure containing a JPG or PNG, in their respective file formats.
    You can use the lh_blob_to_headerfile() function to embed file data in a headerfile if you like.

  const char *lh_load( void *id, lh_callback_t, lh_systemstate* )
    Called when the user loads the library.
    The first parameter is the internal ID of the library. This must
    be given as the first parameter on every call of the callback
    function, or LCDHost won't recognize which library is doing the request.
    The second parameter is the address of the callback function. Save this.
    The third parameter points to information about the current LCDHost system state.
    The information in this structure will change all the time, so don't cache it.
    Return NULL for success, else an error message.

  void lh_unload(void)
    Called when the library is being unloaded.

  int lh_polling()
    Allows you to be called back after a given number of milliseconds.
    Return zero to disable polling. If you just need a regular callback,
    use lh_notify(LH_NOTE_SECOND) instead, as that is more efficient.

  int lh_notify(int code, void* param)
    A library can get the same notifications as an instance can.
    Usually, not many of them are useful to libraries, but if you need
    to poll for devices (when the OS or underlying architecture doesn't
    support event-driven notifications to your driver), you might use
    LH_NOTE_SECOND.

  const lh_class **lh_class_list()
    If your library supplies layout classes, this is how to expose
    them. Return array of pointers to class info, end with NULL pointer.
    If you add or remove classes dynamically, use lh_callback() with
    lh_cb_class_refresh to have LCDHost call lh_class_list() again.

  const lh_buildinfo *lh_get_buildinfo()
    Return the librarys build information, or NULL if none is provided.
    \sa lh_build

  */

#ifndef LH_PLUGIN_H
#define LH_PLUGIN_H

#include <stddef.h>
#include <limits.h>

#include "lh_systemstate.h"

#define LH_LIBRARY_VERSION 3
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

typedef struct lh_class_t lh_class;

/**
  Contains the libraries internal version number, which is a UTF-8 encoded
  string simply shown to the user, the build number which is an increasing
  sequence number used to determine if there is a newer version of the
  library available, and the check and download URLs.

  The check URL should return a simple string (mime type text/plain) with
  the build number available at the download URL.

  The download URL should download the latest version of the plugin.
  */
typedef struct lh_buildinfo_t
{
    const char *version;        /* UTF-8 encoded version string, not interpreted, may be NULL */
    int build;                  /* build number */
    const char *check_url;      /* see above for comments, may be NULL */
    const char *download_url;   /* see above for comments, may be NULL */
} lh_buildinfo;

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
  */
typedef struct lh_device_backlight_t
{
    int r, g, b; /* backlight color setting */
    char is_present; /* device has backlight */
    char is_readable; /* device backlight setting is readable */
    char is_writeable; /* device backlight setting is writable */
} lh_device_backlight;


/**
  Used with lh_callback().
  Add new codes AT THE END, or existing pluins will break.
  */
typedef enum lh_callbackcode_t
{
    lh_cb_unload, /* ask that the library be unloaded, param: NULL or const char *message */

    lh_cb_setup_refresh, /* LCDHost will re-read your setup item, param: lh_setup_item* */
    lh_cb_setup_rebuild, /* use this if you add or remove setup items, LCDHost will call setup_data() again */
    lh_cb_log, /* log an UTF-8 encoded string in the error log */
    lh_cb_polling, /* ask for a call to the polling function, param: NULL */
    lh_cb_notify, /* ask for a call to the notify function, param: NULL */

    /* This request causes LCDHost to call your lh_class_list() function again. */
    lh_cb_class_refresh,

    /* These requests are meaningful only if you supply layout classes and have active instances */
    lh_cb_render, /* ask for a rendering sequence (prerender/width/height/render), param: NULL */

    /* These requests are meaningful only for device drivers */
    lh_cb_arrive, /* a new device have been detected, param is lh_device pointer */
    lh_cb_leave, /* a device has left the system */
    lh_cb_button, /* device button state has changed, param is new bit mask cast to pointer */

    lh_cb_load_layout, /* request loading of a new layout, param: UTF-8 encoded file name relative to data path */

    /* Support calls */
    lh_cb_utf8_to_local8bit, /* request UTF-8 to local 8-bit conversion, param: char *string */
    lh_cb_unused
} lh_callbackcode;

/**
  The library-to-LCDHost callback.
  */
typedef void (*lh_callback_t)( void *id, const void *obj, lh_callbackcode code, void *param );

/* Definition of signature area */
typedef struct lh_signature_t
{
    char marker[16]; /* unique series to allow finding the sig */
    char sign[256]; /* 2048-bit RSA signature of the shared library's SHA-1 digest, PKCS1 padded */
    char url[128]; /* URL to the public key */
    char md5[16]; /* MD5 digest of the shared library except this structure */
    int size; /* sizeof(lh_signature) */
} lh_signature;

#define LH_SIGNATURE_MARKER {7,98,120,242,114,174,176,97,178,246,229,116,243,34,2,92}

/* Declare a signature area - don't mess with the constants, */
/* they're there so that SignLibrary can find the right spot. */
#define LH_SIGNATURE() lh_signature _lh_library_signature = { LH_SIGNATURE_MARKER, {0}, {0}, {0}, sizeof(lh_signature) }

typedef enum lh_setup_type_t
{
    lh_type_none,
    lh_type_integer,
    lh_type_integer_boolean, /* checkbox */
    lh_type_integer_color, /* 32-bit AARRGGBB */
    lh_type_integer_slider,
    lh_type_integer_progress,
    lh_type_integer_list, /* using dropdown box, have user select one of the param.list strings */
    lh_type_fraction,
    lh_type_string, /* all strings are null-terminated, utf-8 encoded */
    lh_type_string_script, /* inline script */
    lh_type_string_filename, /* present the user with an file selection dialog */
    lh_type_string_font, /* simple font selection, see QFont::toString() for string format */
    lh_type_string_inputstate, /* ask user for a specific button state or half axis movement */
    lh_type_string_inputvalue, /* ask user for a button or full axis movement */
    lh_type_image_png, /* allows the display of a PNG image in the setup pane (data.s and param.size used) */
    lh_type_image_qimage, /* allows the display of a QImage in the setup pane static_cast<QImage*>(data.s) */
    lh_type_integer_listbox, /* using listbox, have user select one of the param.list strings */
    lh_type_string_button, /* a clickable button */
    lh_type_last /* marks last used value */
} lh_setup_type;

#define LH_FLAG_READONLY    0x0001 /* Setup item is disabled in GUI */
#define LH_FLAG_HIDDEN      0x0002 /* Setup item is excluded from GUI */
#define LH_FLAG_FOCUS       0x0004 /* Setup item is focused when user double clicks instance */
#define LH_FLAG_AUTORENDER  0x0008 /* Request a render when changed */
#define LH_FLAG_FIRST       0x0010 /* Used with LH_QtSetupItem's - Same as calling setOrder(-1) */
#define LH_FLAG_LAST        0x0020 /* Used with LH_QtSetupItem's - Same as calling setOrder(1) */
#define LH_FLAG_NOSAVE      0x0040 /* Setup item won't be saved to or loaded from layout */
#define LH_FLAG_UNUSED      (~(LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_FOCUS| \
                            LH_FLAG_AUTORENDER|LH_FLAG_FIRST|LH_FLAG_LAST|LH_FLAG_NOSAVE))

typedef union lh_setup_param_t
{
    size_t size;    /**< size of buffer pointed to by 'data.s' */
    struct {
        int min;    /**< int, slider or progress minimum value */
        int max;    /**< int, slider or progress maximum value */
    } slider;
    struct {
        float min;  /**< float minimum value */
        float max;  /**< float maximum value */
    } range;
    const char *list; /**< tab-delimited list of strings */
} lh_setup_param;

typedef union lh_setup_data_t
{
    char *s; /* note that only 's' can be initialized in a global or static, see C syntax rules */
    float f;
    int i;
} lh_setup_data;

/*
 Setup items are the main information link between LCDHost and it's plugins.
 Start a setup item name with '^' to not display the name, leaving the name column blank.
 Start a setup item name with '~' to extend the setup item into the name column.
*/

typedef struct lh_setup_item_t
{
    const char *name; /* name to identify this item uniquely, and display to the user (start with ~ to hide from display */
    const char *help; /* short HTML help text shows as tooltip, may be NULL */
    lh_setup_type type; /* type of data, see enum above */
    int flags; /* LH_FLAG_xxx */
    lh_setup_param param;
    lh_setup_data data;
} lh_setup_item;

/* Common methods to all objects created in libraries */
typedef struct lh_object_calltable_t
{
    int size; // sizeof(lh_object_calltable)
    lh_setup_item ** (*obj_setup_data)(void*); /**< return array of pointers to setup items, NULL terminated */
    void (*obj_setup_resize)(void*, lh_setup_item*, size_t); /**< item data storage is too small, please resize */
    void (*obj_setup_change)(void*, lh_setup_item*); /**< given item has been changed */
    void (*obj_setup_input)(void*, lh_setup_item*, int, int); /**< input item has changed, wanted flags in 'f', new state/value in 'v' */
    int (*obj_polling)(void*); /**< return ms to wait before next call, or zero to stop polling */
    int (*obj_notify)(void*,int,void*); /**< return wanted notification mask, see LH_NOTE_xxx */
} lh_object_calltable;

#define lh_object_calltable_NULL { sizeof(lh_object_calltable), 0, 0, 0, 0, 0, 0 }

/**
  This structure is what defines a driver device methods. It's embedded in
  the \c lh_device structure. Members in this structure may be set to
  NULL. Return values are either NULL, to indicate success, or pointers
  to an UTF-8 encoded error string (either in English or localized).
  The exception to this is the \c obj_buttons function, which returns the
  current button state.

  IMPORTANT! Since the driver creates and destroys devices when it needs,
  you MUST let LCDHost know that a device is dying before it's pointer
  goes stale, otherwise you'll crash the driver. Similarly, don't advertise
  a device until it's ready to accept calls from LCDHost.

  \code
    myDevice = new myDevice();
    lh_callback(id, myDevice, lh_drv_arrive, myDevice->devtable );
    lh_callback(id, myDevice, lh_drv_leave, myDevice->devtable );
    delete myDevice;
  \endcode

  */
typedef struct lh_device_calltable_t
{
    int size; // sizeof(lh_device_calltable)
    lh_object_calltable o;
    const char* (*obj_open)(void*); /**< device is selected by user for output */
    const char* (*obj_render_qimage)(void*,void*); /**< render a QImage onto the device */
    const char* (*obj_render_argb32)(void*,int,int,const void*); /**< render an ARB32 bitmap onto the device */
    const char* (*obj_render_mono)(void*,int,int,const void*); /**< render a monochrome one-byte-per-pixel map */
    int (*obj_buttons)(void*); /**< return current button state as a bitmap */
    const char* (*obj_get_backlight)(void*,lh_device_backlight*); /**< return current backlight state */
    const char* (*obj_set_backlight)(void*,lh_device_backlight*); /**< change current backlight state */
    const char* (*obj_close)(void*); /**< device is no longer selected for output */
} lh_device_calltable;

/**
  This structure gives basic information about a device.

  \c name is the text to present to the user as the
  devices name, for example 'Logitech G19'. This text should
  be localized to the users language, or be in English.
  */
typedef struct lh_device_t
{
    int size; /* sizeof(lh_device) */
    const char *id; /* UTF-8 encoded, uniquely identifies the device with the driver across reboots */
    const char *name; /* UTF-8 encoded, should be localized */
    int width; /* width in pixels */
    int height; /* height in pixels */
    int depth; /* bit depth */
    int noauto; /* don't autoselect this device; manual selection only */
    const char *button[LH_DEVICE_MAXBUTTONS]; /* UTF-8 encoded softbutton names (may be NULL) */
    lh_blob *logo; /* device logo or image */
    lh_device_calltable table;
} lh_device;

/**
  This structure is what defines a layout class methods. It's embedded in the
  \c lh_class structure. Members in this structure may be set to NULL.
  */
typedef struct lh_instance_calltable_t
{
    int size; // sizeof(lh_instance_calltable)
    lh_object_calltable o;
    void * (*obj_new)(const char*,const lh_class*); /**< return a new instance of the class (name,class) */
    void (*unused_1)(); /** unused, left in for binary compatibility (was: obj_sizepos) */
    void (*obj_prerender)(void*); /**< called right before width/height/render_xxx as a notification */
    int (*obj_width)(void*,int); /**< return suggested width given a height (or -1 for default width) */
    int (*obj_height)(void*,int); /**< return suggested height given a width (or -1 for default height) */
    const lh_blob * (*obj_render_blob)(void*,int,int); /**< render object to any image format using width x height */
    void * (*obj_render_qimage)(void*,int,int); /**< render object to QImage using width x height */
    void (*obj_delete)(void*); /**< delete this instance of the class */
} lh_instance_calltable;

#define lh_instance_calltable_NULL { sizeof(lh_instance_calltable), lh_object_calltable_NULL, 0,0,0,0,0,0,0,0 }

/**
  This structure gives basic information about the class.

  Both the path and name members are UTF-8 encoded and NULL terminated strings.

  \c path is where to place the class in the class list tree. It must be in
  english, and LCDHost may attempt to translate the path items into the users
  language. Example: '/System/Date and time'

  \c id is the internal identifier for this class. It should remain static on
  all platforms/languages. The name of the shared library plus this will be
  used to identify the class uniquely.

  \c name is the text to present to the user as the class' name, for example
  'Clock'. This text should be localized to the users language.

  The width/height members are used to estimate the size of an instance before
  actually creating it. Set to -1 if you have no idea.
  */
struct lh_class_t
{
    int size; /* sizeof(lh_class) */
    const char *path; /* UTF-8 encoded, must NOT be localized */
    const char *id;   /* UTF-8 encoded, must NOT be localized */
    const char *name; /* UTF-8 encoded, should be localized */
    int width;
    int height;
    lh_instance_calltable table;
};


#ifdef __cplusplus
class lh_library_calltable
{
public:
    const char * (*lh_name) (void); /* Required */
    const char * (*lh_shortdesc) (void); /* Required */
    int (*lh_version) (int); /* Required */
    const char * (*lh_author) (void);
    const char * (*lh_homepage) (void);
    const char * (*lh_longdesc) (void);
    const lh_blob * (*lh_logo) (void); /* return blob containing JPG or PNG */
    const char * (*lh_load)(void *id, lh_callback_t, lh_systemstate*); /* return NULL for success, else error message */
    void (*lh_unload)(void);

    /* Optional, but might be useful */
    int (*lh_polling)();
    int (*lh_notify)(int,void*);

    /* Meaningful only for libraries supplying layout classes */
    const lh_class ** (*lh_class_list)(void); /* return array of pointers to class info, end with NULL pointer */
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Utility functions in lh_plugin.c */
lh_blob *lh_binaryfile_to_blob( const char *filename ); /* Caller must free() the blob */
void lh_blob_to_headerfile( lh_blob *blob, const char *filename, const char *varname );
int lh_cpuload( lh_cpudata *from, lh_cpudata *to ); /* Returns average load between those times as permille */

#ifdef __cplusplus
}
#endif


#endif /* LH_PLUGIN_H */
