#ifndef LH_HWMONDATA_H
#define LH_HWMONDATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"

#include <QSettings>

class LH_HWMonData: public LH_MonitoringData
{
    void loadSensorList(QSettings *reg);
    QList< QList<QString> > sensorKeys_;

    bool getSelectedValue(float& value, QString& text, QString& units, int& count, int index);

protected:
    bool getCount(int& count);
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);
    bool getData(float& value, QString& text, QString& units, int& count, int index);

public:
    LH_HWMonData( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false);

    QString appName() { return "HWMonitor + HWMonTray"; }
    monitoringDataType getType();

};

#endif // LH_HWMONDATA_H
