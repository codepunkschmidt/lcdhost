#ifndef LH_RIVATUNERDATA_H
#define LH_RIVATUNERDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_RivaTunerTypes.h"

class LH_RivaTunerData: public LH_MonitoringData
{
    void getSelectedValue(RTHM_SHARED_MEMORY_HEADER* RTHMHeader, float& value, QString& text, QString& units, int index);
    void loadTypesList(RTHM_SHARED_MEMORY_HEADER* RTHMHeader);
    QString reverse(QString str);

    bool is_avg_;
protected:
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);

public:
    LH_RivaTunerData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false );
    ~LH_RivaTunerData();

    QString appName() { return "RivaTuner"; }
    monitoringDataType getType();
    bool getCount(int& count);
    bool getNames(QStringList& names);
    int lookupIndex(int lineID);

    void loadItemsList(int i);
};

#endif // LH_RIVATUNERDATA_H
