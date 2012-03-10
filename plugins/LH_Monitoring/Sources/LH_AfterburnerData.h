#ifndef LH_AFTERBURNERDATA_H
#define LH_AFTERBURNERDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_AfterburnerTypes.h"

class LH_AfterburnerData: public LH_MonitoringData
{
    void getSelectedValue(MAHM_SHARED_MEMORY_HEADER* MAHMHeader, float& value, QString& text, QString& units, QString& format, float& min, float& max, int index);
    void loadTypesList(MAHM_SHARED_MEMORY_HEADER* MAHMHeader);
    QString reverse(QString str);

    bool is_avg_;
protected:
    //LH_Qt_QString *setup_value_count_;
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    bool getData(float& value, QString& text, QString& units, QString& format, float& min, float& max, int index);

public:

    //LH_Qt_bool *setup_value_format_;

    LH_AfterburnerData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );

    QString appName() { return "MSI Afterburner"; }
    monitoringDataType getType();
    bool getCount(int& count);
    bool getNames(QStringList& names);
    int lookupIndex(int lineID);
    QString getText(bool addUnits);
    bool getGraphMin(float& min);
    bool getGraphMax(float& max);

};

#endif // LH_AFTERBURNERDATA_H
