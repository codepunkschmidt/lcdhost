#ifndef LH_MONITORINGDATA_H
#define LH_MONITORINGDATA_H

#include <QObject>
#include "../../LH_QtObject.h"
#include <QStringList>

class LH_MonitoringUI;

enum monitoringDataMode
{
    mdmNumbers = 1,
    mdmText = 2,
    mdmAll = mdmNumbers + mdmText
};

enum monitoringDataType
{
    mdtInt = 0,
    mdtFloat = 1,
    mdtText = 2
};

class LH_MonitoringData : public QObject
{
protected:
    int sel_id_all_;
    int sel_id_avg_;

    bool includeGroups_;
    bool is_group_;

    LH_MonitoringUI *ui_;

    // get the selected data item
    virtual bool getData(float& , QString& , QString& ) { return false; }
    // get the specified data item (for grouped data items, e.g. averages etc)
    virtual bool getData(float& , QString& , QString& , int ) { return false; }

    float getValueFromSet(int index, int count, const float *valueSet);
    float getValueFromSet(int index, int count, const unsigned int *valueSet);
    float getValueFromSet(int index, int count, const int *valueSet);
    //template<typename T> float getValueFromSet(int& index, int& count, T valueSet[]);

public:
    LH_MonitoringData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups = false);

    virtual QString appName() { /*Q_Assert(false);*/ return "Unknown"; }  //must override

    virtual bool getCount(int& count) { count = 1; return false; }
    virtual QString getText(bool addUnits, bool round = false);
    virtual QString getUnits();
    virtual monitoringDataType getType() {return mdtInt;}
    virtual int getThreshMin() {return 0;}
    virtual int getThreshMax();
    virtual bool getGraphMin(float& ) {return false;}
    virtual bool getGraphMax(float& ) {return false;}
    virtual bool getNames(QStringList& names);
    virtual bool getValue(float& value);
    virtual bool getValue(float& value, int index);

    virtual int lookupIndex(int index) {return index;}

    // if a sensor reports a specific value when the sensor is dead or disconnected,
    // define the value here:
    virtual bool getDeadValue(float& ) {return false;}
    // if the data value has to be transformed before plotting, apply the transformation
    // here (e.g. speedfan reports all temperature values multiplied 100, so these need
    // to be divided by 100 before displaying them)
    virtual bool getDeadValue_Transformed(float& value) {return getDeadValue(value);}

    virtual bool isGroup() { return is_group_; }
    virtual void setIsGroup(bool b) { is_group_  = b; }
};

#endif // LH_MONITORINGDATA_H

