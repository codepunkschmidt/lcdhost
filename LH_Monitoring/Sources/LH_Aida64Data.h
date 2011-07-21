#ifndef LH_AIDA64DATA_H
#define LH_AIDA64DATA_H

#include "LH_MonitoringData.h"
#include "LH_MonitoringUI.h"
#include "LH_Aida64Types.h"

#include <QtXml>

class LH_Aida64Data: public LH_MonitoringData
{
    void saveXML(QString fileName);
    QDomDocument aidaXml_;
    bool dataLoaded_;
    monitoringDataMode dataMode_;

    sensorTypesList sensorTypes_;

    void getSelectedValue(QDomElement docElem, float& value, QString& text, QString& units, int index);

protected:
    bool getData(float& value, QString& text, QString& units);
    bool getData(float& value, QString& text, QString& units, int index);

public:
    void updateLists();
    bool loadXML(bool doListUpdate = true);

    LH_Aida64Data( LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode = mdmNumbers, bool includeGroups = false);
    ~LH_Aida64Data();

    QString appName() { return "Aida64"; }
    bool getCount(int& count);
    bool getNames(QStringList& names);
    monitoringDataType getType();
    int getThreshMax();
};

#endif // LH_AIDA64DATA_H
