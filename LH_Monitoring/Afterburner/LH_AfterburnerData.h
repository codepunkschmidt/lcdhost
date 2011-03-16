#ifndef LH_AFTERBURNERDATA_H
#define LH_AFTERBURNERDATA_H

#include "../_Templates/LH_MonitoringData.h"
#include <windows.h>

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

struct MAHM_SHARED_MEMORY_ENTRY
{
       char	szSrcName[MAX_PATH];
               //data source name (e.g. "Core clock")
       char	szSrcUnits[MAX_PATH];
               //data source units (e.g. "MHz")

       char	szLocalizedSrcName[MAX_PATH];
               //localized data source name (e.g. "„астота €дра" for Russian GUI)
       char	szLocalizedSrcUnits[MAX_PATH];
               //localized data source units (e.g. "ћ√ц" for Russian GUI)

       char	szRecommendedFormat[MAX_PATH];
               //recommended output format (e.g. "%.3f" for "Core voltage" data source)

       float	data;
               //last polled data (e.g. 500MHz)
               //(this field can be set to FLT_MAX if data is not available at
               //the moment)
       float	minLimit;
               //minimum limit for graphs (e.g. 0MHz)
       float	maxLimit;
               //maximum limit for graphs (e.g. 2000MHz)

       DWORD	dwFlags;
               //bitmask containing combination of MAHM_SHARED_MEMORY_ENTRY_FLAG_...

};

struct sensorGroup
{
    QStringList names;
    QList<int> indexes;
};

class LH_AfterburnerData: public LH_MonitoringData
{
    void getSelectedValue(MAHM_SHARED_MEMORY_HEADER* MAHMHeader, float& value, QString& text, QString& units, QString& format, float& min, float& max, int index);
    void loadTypesList(MAHM_SHARED_MEMORY_HEADER* MAHMHeader);
    QString reverse(QString str);

    QList<sensorGroup> sensors_;

    int getIndex();
    void setIndex(int index = -1);

    bool is_avg_;
protected:
    LH_Qt_QString *setup_value_count_;
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    bool getData(float& value, QString& text, QString& units, QString& format, float& min, float& max, int index);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_QStringList *setup_value_item_;
    LH_Qt_int *setup_value_index_;

    LH_Qt_bool *setup_value_format_;

    LH_AfterburnerData( LH_QtObject *parent, bool includeGroups = false );
    ~LH_AfterburnerData();

    QString appName() { return "MSI Afterburner"; }
    monitoringDataType getType();
    bool getCount(int& count);
    bool getNames(QStringList& names);
    int lookupIndex(int lineID);
    QString getText(bool addUnits);
    bool getGraphMin(float& min);
    bool getGraphMax(float& max);

    void setTypeSelection();
    void changeTypeSelection();
    void setItemSelection();
    void changeItemSelection();

    void loadItemsList(int i);
};

#endif // LH_AFTERBURNERDATA_H
