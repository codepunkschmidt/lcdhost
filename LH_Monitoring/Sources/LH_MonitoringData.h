#ifndef LH_MONITORINGDATA_H
#define LH_MONITORINGDATA_H

#include <QObject>
#include "LH_QtObject.h"
#include <QStringList>
#include <QHash>
#include "qmath.h"

class LH_MonitoringUI;

enum monitoringDataMode
{
    mdmNumbers = 1,
    mdmText = 2,
    mdmAll = mdmNumbers + mdmText,
    mdmPie = 4
};

enum monitoringDataType
{
    mdtInt = 0,
    mdtFloat = 1,
    mdtText = 2
};

class unitOptionsType
{
    int divisor_;
    QStringList options_;
    QHash<QString, int> lookup_;
public:
    QString convert(float &value, QString unit = "")
    {
        if(divisor_!=0)
        {
            int i = 0;
            if(unit=="")
            {
                unit = options_[0];
                while(value >= divisor_ && ++i<options_.length())
                {
                    value /= divisor_;
                    unit = options_[i];
                }
            }
            else
                value /= qPow(divisor_, lookup_.value(unit, 0));
        }
        return unit;
    }

    QStringList list() { return QStringList(options_); }

    unitOptionsType(int divisor = 0, QStringList *options = NULL)
    {
        set(divisor, options);
    }

    void addAuto()
    {
        options_.insert(0,"(Auto)");
        lookup_.insert("(Auto)",-1);
    }

    void set(int divisor = 0, QStringList *options = NULL)
    {
        if(!options)
            divisor_ = 0;
        else
        {
            divisor_ = divisor;
            for(int i = 0; i<options->length(); i++)
            {
                options_.append(options->at(i));
                lookup_.insert(options->at(i),i);
            }
        }
    }
};

class LH_MonitoringData : public QObject
{
protected:
    int sel_id_all_;
    int sel_id_avg_;

    bool includeGroups_;
    bool is_group_;
    bool adaptiveUnitsAllowed_;
    bool adaptiveUnits_;
    QString desiredUnits_;

    LH_MonitoringUI *ui_;
    monitoringDataMode dataMode_;

    // get the selected data item
    virtual bool getData(float& , QString& , QString& ) { return false; }
    // get the specified data item (for grouped data items, e.g. averages etc)
    virtual bool getData(float& , QString& , QString& , int ) { return false; }

    float getValueFromSet(int index, int count, const float *valueSet);
    float getValueFromSet(int index, int count, const unsigned int *valueSet);
    float getValueFromSet(int index, int count, const int *valueSet);
    //template<typename T> float getValueFromSet(int& index, int& count, T valueSet[]);

public:
    LH_MonitoringData(LH_QtObject *parent, LH_MonitoringUI *ui, monitoringDataMode dataMode, bool includeGroups = false, bool allowAdaptiveUnits = false);

    virtual QString appName() { /*Q_Assert(false);*/ return "Unknown"; }  //must override

    virtual bool getCount(int& count) { count = 1; return false; }
    virtual QString getText(bool addUnits, bool round = false);
    virtual QString getUnits();
    virtual monitoringDataType getType() {return mdtInt;}
    virtual int getThreshMin() {return 0;}
    virtual int getThreshMax();
    virtual bool getGraphMin(float& ) {return false;}
    virtual bool getGraphMax(float& ) {return false;}
    virtual bool getUpperLimit(float& ) {return false;}
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

    virtual bool getAdaptiveUnitOptions(unitOptionsType &options) {Q_UNUSED(options); return false;}
    virtual bool adaptiveUnitsAllowed() { return adaptiveUnitsAllowed_; }
    virtual bool adaptiveUnits() { return adaptiveUnits_; }
    virtual void setAdaptiveUnits(bool b) { adaptiveUnits_ = b; }
    virtual QStringList adaptiveUnitOptions()  {
        unitOptionsType options;
        if(getAdaptiveUnitOptions(options))
            return options.list();
        else
            return QStringList();
    }
    virtual bool setUnits(QString s) {
        if(!adaptiveUnitsAllowed_) return false;
        setAdaptiveUnits(false);
        desiredUnits_ = s;
        return true;
    }

};

#endif // LH_MONITORINGDATA_H

