#ifndef LH_RIVATUNERDATA_H
#define LH_RIVATUNERDATA_H

#include "../_Templates/LH_MonitoringData.h"

#ifdef Q_WS_WIN
# include <windows.h>
#endif

#ifndef DWORD
# define DWORD quint32
#endif

struct RTHM_SHARED_MEMORY_HEADER
{
    DWORD	dwSignature;
            //signature allows applications to verify status of shared memory

            //The signature can be set to:
            //'RTHM'	- hardware monitoring memory is initialized and contains
            //			valid data
            //0xDEAD	- hardware monitoring memory is marked for deallocation and
            //			no longer contain valid data
            //otherwise the memory is not initialized
    DWORD	dwVersion;
            //header version ((major<<16) + minor)
            //must be set to 0x00010001 or greater to use this structure
    DWORD	dwNumEntries;
            //number of subsequent RTHM_SHARED_MEMORY_ENTRY entries
    time_t	time;
            //last polling time
    DWORD	dwEntrySize;
            //size of entries in subsequent RTHM_SHARED_MEMORY_ENTRY entries array
};
#define	RTHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_OSD   0x00000001 //item is configured to display in On-Screen Display
#define	RTHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_LCD   0x00000002 //item is configured to display in Logitech keyboard LCD
#define	RTHM_SHARED_MEMORY_ENTRY_FLAG_SHOW_IN_TRAY  0x00000004 //item is configured to display in tray icon

struct RTHM_SHARED_MEMORY_ENTRY
{
    char	czSrc[32];
            //data source name (e.g. "Core temperature")
    char	czDim[16];
            //data source dimansion (e.g. "°C")
    float	data;
            //last polled data in raw format (e.g. 50°C)
            //(this field can be set to FLT_MAX if data is not available)

            //take a note that the user can enable raw data transforming mode and
            //display different values on the graph (e.g. raw core VID values can
            //be translated to mapped core voltages).
            //In this case you may use dataTransformed field to get transformed value
    float	offset;
            //user specified data offset (e.g. +10°C temerature compensation)

            //take a note that this value is not automatically added to transformed
            //data, it is up to you to add offset to it
    float	dataTransformed;
            //last polled data in transformed format
    DWORD	flags;
            //bitmask, containing combination of RTHM_SHARED_MEMORY_ENTRY_FLAG_... flags
};

struct sensorGroup
{
    QStringList names;
    QList<int> indexes;
};

class LH_RivaTunerData: public LH_MonitoringData
{
    void getSelectedValue(RTHM_SHARED_MEMORY_HEADER* RTHMHeader, float& value, QString& text, QString& units, int index);
    void loadTypesList(RTHM_SHARED_MEMORY_HEADER* RTHMHeader);
    QString reverse(QString str);

    QList<sensorGroup> sensors_;

    int getIndex();
    void setIndex(int index = -1);

    bool is_avg_;
protected:
    LH_Qt_QString *setup_value_count_;
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_QStringList *setup_value_item_;
    LH_Qt_int *setup_value_index_;

    LH_Qt_bool *setup_value_offset_;
    LH_Qt_bool *setup_value_format_;

    LH_RivaTunerData( LH_QtObject *parent, bool includeGroups = false );
    ~LH_RivaTunerData();

    QString appName() { return "RivaTuner"; }
    monitoringDataType getType();
    bool getCount(int& count);
    bool getNames(QStringList& names);
    int lookupIndex(int lineID);

    void setTypeSelection();
    void changeTypeSelection();
    void setItemSelection();
    void changeItemSelection();

    void loadItemsList(int i);
};

#endif // LH_RIVATUNERDATA_H
