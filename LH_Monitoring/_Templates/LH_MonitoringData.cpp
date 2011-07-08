#include "LH_MonitoringData.h"
#include <QDebug>

LH_MonitoringData::LH_MonitoringData(LH_QtObject *parent, bool includeGroups) : QObject(parent)
{
    includeGroups_ = includeGroups;
    is_group_ = false;
}

LH_MonitoringData::~LH_MonitoringData()
{

}

int LH_MonitoringData::getThreshMax()
{
    QString units = getUnits();

    if(units == "MHz")
        return 99999;
    if(units == "°C")
        return 200;
    if(units == "%")
        return 100;
    if(units == "RPM")
        return 99999;
    if(units == "V")
        return 20;
    if(units == "FPS")
        return 999;

    return 99999;
}

bool LH_MonitoringData::getNames(QStringList& names) {
    names.clear();
    int count = 1;
    if (!getCount(count)) count = 1;
    if(count == 1 )
        names.append("Default");
    else
        for (int i = 0; i < count; i++)
            names.append(QString("Item #%1").arg(i+1));
    return false;
}

bool LH_MonitoringData::getValue(float& value)
{
    QString units;
    QString text;
    return (getData(value, text, units));
}

bool LH_MonitoringData::getValue(float& value, int index)
{
    QString units;
    QString text;
    return (getData(value, text, units, index));
}

QString LH_MonitoringData::getText(bool addUnits, bool round)
{
    QString units;
    float value;
    QString resultText;
    if (getData(value, resultText, units))
    {
        if(round || getType()==mdtInt)
            value = int(value > 0.0 ? value + 0.5 : value - 0.5);
        if (resultText=="")
            resultText = QString("%1").arg(value);

        if(addUnits) resultText = resultText + units;
    } else
        resultText = "N/A";

    return resultText;
}

QString LH_MonitoringData::getUnits()
{
    QString units = "";
    QString text = "";
    float value;
    getData(value, text, units);
    return units;
}

float LH_MonitoringData::getValueFromSet(int index, int count, const float *valueSet)
{
    float deadVal = 0;
    bool hasDeadVal = (getDeadValue(deadVal));

    float value;
    if(index>=count && count>0)
    {
        int validCount = 0;
        value = (index == count? 0 : valueSet[0]);
        for(int i=0; i<count; i++)
        {
            if(hasDeadVal && (deadVal == valueSet[i])) continue;
            validCount ++;
            if (index == count) //Average
                value += valueSet[i];
            else if (index == count + 1)
                value = qMin(value, valueSet[i]);
            else
                value = qMax(value, valueSet[i]);
        }
        if (index == count) value = qRound((value*10.0) / validCount)/10.0;
    } else
        value = valueSet[index];
    return value;
}
float LH_MonitoringData::getValueFromSet(int index, int count, const unsigned int *valueSet)
{
    float deadVal = 0;
    bool hasDeadVal = (getDeadValue(deadVal));

    float value;
    if(index>=count && count>0)
    {
        int validCount = 0;
        value = (index == count? 0 : valueSet[0]);
        for(int i=0; i<count; i++)
        {
            if(hasDeadVal && (deadVal == valueSet[i])) continue;
            validCount ++;
            if (index == count) //Average
                value += valueSet[i];
            else if (index == count + 1)
                value = qMin(value, (float)valueSet[i]);
            else
                value = qMax(value, (float)valueSet[i]);
        }
        if (index == count) value = qRound((value*10.0) / validCount)/10.0;
    } else
        value = valueSet[index];
    return value;
}
float LH_MonitoringData::getValueFromSet(int index, int count, const int *valueSet)
{
    float deadVal = 0;
    bool hasDeadVal = (getDeadValue(deadVal));

    float value;
    if(index>=count && count>0)
    {
        int validCount = 0;
        value = (index == count? 0 : valueSet[0]);
        for(int i=0; i<count; i++)
        {
            if(hasDeadVal && (deadVal == valueSet[i])) continue;
            validCount ++;
            if (index == count) //Average
                value += valueSet[i];
            else if (index == count + 1)
                value = qMin(value, (float)valueSet[i]);
            else
                value = qMax(value, (float)valueSet[i]);
        }
        if (index == count) value = qRound((value*10.0) / validCount)/10.0;
    } else
        value = valueSet[index];
    return value;
}
