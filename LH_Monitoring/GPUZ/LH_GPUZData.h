#ifndef LH_GPUZDATA_H
#define LH_GPUZDATA_H

#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_bool.h"
#include "../LH_Qt_int.h"

#ifdef Q_WS_WIN
# include <windows.h>
#endif
#include "../_Templates/LH_MonitoringData.h"

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

class LH_GPUZData: public LH_MonitoringData
{
    void getSelectedValue(GPUZ_SH_MEM* gpuzMemory, float& value, QString& text, QString& units);
    void loadTypesList(GPUZ_SH_MEM* gpuzMemory);

protected:
    LH_Qt_QString *setup_value_count_;
    bool getData(float& value, QString& text, QString& units);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_int *setup_value_type_index_;

    LH_GPUZData( LH_QtObject *parent );
    ~LH_GPUZData();

    QString appName() { return "GPU-Z"; }
    monitoringDataType getType();

    void setTypeSelection();
    void changeTypeSelection();
};

#endif // LH_GPUZDATA_H
