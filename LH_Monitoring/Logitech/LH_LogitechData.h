#ifndef LH_LOGITECHDATA_H
#define LH_LOGITECHDATA_H

#include "../_Templates/LH_MonitoringData.h"
#include <QSettings>
#include <QList>

extern int logitechEnabled_;

class LH_LogitechData: public LH_MonitoringData
{
    QList<QStringList> batteryStates;
protected:
    bool getData(float& value, QString& text, QString& units);

public:
    LH_Qt_QStringList *setup_value_type_;
    LH_Qt_int *setup_value_type_index_;

    LH_LogitechData( LH_QtObject *parent, monitoringDataMode dataMode = mdmNumbers);
    ~LH_LogitechData();

    QString appName() { return "Logitech"; }
    monitoringDataType getType();

    void setTypeSelection();
    void changeTypeSelection();

    static bool monitoringEnabled();
};

#endif // LH_LOGITECHDATA_H
