#ifndef LH_AFTERBURNERTYPES_H
#define LH_AFTERBURNERTYPES_H

#include <QtGlobal>

#ifdef Q_OS_WIN
# include <windows.h>
#endif

struct MAHM_SHARED_MEMORY_HEADER
{
        DWORD	dwSignature;
                //signature allows applications to verify status of shared memory

                //The signature can be set to:
                //'MAHM'	- hardware monitoring memory is initialized and contains
                //			valid data
                //0xDEAD	- hardware monitoring memory is marked for deallocation and
                //			no longer contain valid data
                //otherwise the memory is not initialized
        DWORD	dwVersion;
                //header version ((major<<16) + minor)
                //must be set to 0x00010000 for v1.0
        DWORD	dwHeaderSize;
                //size of header
        DWORD	dwNumEntries;
                //number of subsequent MAHM_SHARED_MEMORY_ENTRY entries
        DWORD	dwEntrySize;
                //size of entries in subsequent MAHM_SHARED_MEMORY_ENTRY entries array
        time_t	time;
                //last polling time
};
#define	MAHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_OSD   0x00000001 //item is configured to display in On-Screen Display
#define	MAHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_LCD   0x00000002 //item is configured to display in Logitech keyboard LCD
#define	MAHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_TRAY  0x00000004 //item is configured to display in tray icon

#ifndef MAX_PATH
# define MAX_PATH 260
#endif

struct MAHM_SHARED_MEMORY_ENTRY
{
       char	szSrcName[MAX_PATH];
               //data source name (e.g. "Core clock")
       char	szSrcUnits[MAX_PATH];
               //data source units (e.g. "MHz")

       char	szLocalizedSrcName[MAX_PATH];
               //localized data source name
       char	szLocalizedSrcUnits[MAX_PATH];
               //localized data source units

       char	szRecommendedFormat[MAX_PATH];
               //recommended output format (e.g. "%.3f" for "Core voltage" data source)

       qreal	data;
               //last polled data (e.g. 500MHz)
               //(this field can be set to FLT_MAX if data is not available at
               //the moment)
       qreal	minLimit;
               //minimum limit for graphs (e.g. 0MHz)
       qreal	maxLimit;
               //maximum limit for graphs (e.g. 2000MHz)

       DWORD	dwFlags;
               //bitmask containing combination of MAHM_SHARED_MEMORY_ENTRY_FLAG_...

};

#endif // LH_AFTERBURNERTYPES_H
