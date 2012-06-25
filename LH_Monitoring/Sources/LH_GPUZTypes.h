#ifndef LH_GPUZTYPES_H
#define LH_GPUZTYPES_H

#ifdef Q_WS_WIN
# include <windows.h>
#endif

// pragma pack is included here because the struct is a pascal Packed Record,
// meaning that fields aren't aligned on a 4-byte boundary. 4 bytes fit 2
// 2-byte records.
#define MAX_RECORDS 128

#pragma pack(1)
// This is the struct we're using to access the shared memory.
struct GPUZ_RECORD
{
wchar_t key[256];
wchar_t value[256];
};

struct GPUZ_SENSOR_RECORD
{
wchar_t name[256];
wchar_t unit[8];
quint32 digits;
double value;
};

struct GPUZ_SH_MEM
{
quint32 version; // Version number, 1 for the struct here
volatile long busy;	 // Is data being accessed?
quint32 lastUpdate; // GetTickCount() of last update
GPUZ_RECORD data[MAX_RECORDS];
GPUZ_SENSOR_RECORD sensors[MAX_RECORDS];
};
#pragma pack()

#endif // LH_GPUZTYPES_H
