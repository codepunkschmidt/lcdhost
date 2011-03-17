#ifndef LH_SPEEDFANDATA_H
#define LH_SPEEDFANDATA_H

#include "../_Templates/LH_MonitoringData.h"

#pragma pack(1)
struct SFMemory {
        quint32 version;
        quint32 flags;
        int MemSize;
        int handle;
        quint32 NumTemps;
        quint32 NumFans;
        quint32 NumVolts;
        signed int temps[32];
        signed int fans[32];
        signed int volts[32];
};
#pragma pack()

class LH_SpeedFanData: public LH_MonitoringData
{
    void getSelectedValue(SFMemory* sfmemory, float& value, QString& text, QString& units, int& count, int index);
    void loadSensorList(SFMemory* sfmemory);

    bool is_avg_;
protected:
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    bool getData(float& value, QString& text, QString& units, int& count, int index);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_int *setup_value_type_index_;
    LH_Qt_QStringList *setup_value_index_;
    LH_Qt_int *setup_value_index_index_;

    LH_SpeedFanData( LH_QtObject *parent, bool includeGroups = false );
    ~LH_SpeedFanData();

    QString appName() { return "SpeedFan"; }
    monitoringDataType getType();
    int getThreshMax();
    bool getCount(int& count);
    bool getNames(QStringList& names);
    bool getDeadValue(float& value);
    bool getDeadValue_Transformed(float& value);

    void setTypeSelection();
    void setItemSelection();
    void changeTypeSelection();
    void changeItemSelection();
};

#endif // LH_SPEEDFANDATA_H
