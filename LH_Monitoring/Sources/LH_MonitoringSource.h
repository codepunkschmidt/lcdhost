#ifndef LH_MONITORINGSOURCE_H
#define LH_MONITORINGSOURCE_H

#include <QObject>
#include <QDebug>
#include <QBasicTimer>
#include <QVariant>
#include <QList>
#include <QTimerEvent>
#include <QStringList>
#include "LH_QtObject.h"

#include "LH_Qt_bool.h"
#include "LH_Qt_float.h"
#include "LH_Qt_int.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QSlider.h"

#include "../DataCollection/DataCollection.h"

#include "../json/json.h"


struct OptionalValue
{
    bool exists;
    float value;
    OptionalValue(bool e=false, float v=0.0) :
        exists(e), value(v)
    {}
    bool operator ==(const OptionalValue &other) const
    {
        return exists == other.exists && value == other.value;
    }
};

struct minmax
{
    OptionalValue minimum;
    OptionalValue maximum;
    minmax(const OptionalValue &v1=OptionalValue(), const OptionalValue &v2=OptionalValue()) :
        minimum(v1), maximum(v2)
    {}
    bool operator ==(const minmax &other) const
    {
        return minimum == other.minimum && maximum == other.maximum;
    }
};

typedef DataLine SensorItem ;

struct SensorGroup
{
    QString name;
    DataLineCollection items;
    minmax limits;
    SensorGroup(const QString &n = QString(),
                const DataLineCollection &i = DataLineCollection(),
                const minmax &l = minmax()) :
        name(n), items(i), limits(l)
    {}
};
typedef QHash<QString, SensorGroup> SensorGroups;

struct SensorType
{
    QString name;
    SensorGroups groups;
    SensorType(const QString &n=QString(),
               const SensorGroups &g=SensorGroups()) :
        name(n), groups(g)
    {}
};
typedef QHash<QString, SensorType> SensorTypes;

struct SensorDefinition
{
    QString units;
    minmax limits;
    OptionalValue deadValue;
    SensorDefinition(const QString &u=QString(),
                     const minmax &l=minmax(),
                     const OptionalValue &d=OptionalValue()) :
        units(u), limits(l), deadValue(d)
    {}
};

class LH_MonitoringSource : public LH_QtObject
{
    Q_OBJECT

    QBasicTimer pollTimer_;
    QHash<QString, LH_QtSetupItem*> setupItems_;
    QHash<QString, QMetaType::Type> setupTypes_;
    bool needsRebuild_;

    void updateValueItem(QString key, QVariant val, QMetaType::Type valueType = QMetaType::Void, const char *mimeType = NULL);
    QVariant getValueItem(QString key);

    QString optionsData_;
    bool dataAvailable_;
    bool alwaysShowAllSelectors_;
    QString appName_;

protected:
    LH_Qt_bool* setup_enabled_;
    LH_Qt_QSlider* setup_rate_;

    OptionalValue NA_;

    void updateAggregates(QString type, QString group);

    void setDataAvailable(bool dataAvailable) {
        if(dataAvailable_ != dataAvailable)
        {
            dataAvailable_ = dataAvailable;
            clearSensors();
            emit availabiltyChanged();
        }
    }

    SensorTypes sensors_;

    QString reverse(QString str);
    void clearSensors();
    void updateValue(QString type, QString group, QString item, QVariant val);
    void updateValue(QString type, QString group, QString item, QVariant val, SensorDefinition def);
    void updateValue(QString type, QString group, QString item, QVariant val, SensorDefinition def, bool isAggregate, bool isGroup);
    void timerEvent(QTimerEvent *event)
    {
        if (event->timerId() != pollTimer_.timerId())
            LH_QtObject::timerEvent(event);
        else
        {
            if(int t = polling())
                pollTimer_.start( t, this );
            else
                pollTimer_.stop();
        }
    }

    virtual void beforeUpdate() { return;}
    virtual bool doUpdate() { Q_ASSERT_X(false, "LH_MonitoringSource::update", "Update procedure not defined!"); return false;}
    virtual void afterUpdate();

    int pollingRate()
    {
        int val = setup_rate_->value();
        int ms = 1000;
        if(val < 0)
            ms = 1000 * (1-val);
        if(val > 0)
            ms = qRound(1000.0 / (val+1));
        return ms;
    }

public:
    QString getLinkPath(QString type, QString group, QString item)
    {
        return QString("/%1/%2/%3/%4/%5").arg(STRINGIZE(MONITORING_FOLDER)).arg(appName()).arg(type).arg(group).arg(item).replace("//","/");
    }

    LH_MonitoringSource(LH_QtObject *parent, QString appName, bool alwaysShowAllSelectors = false);

    bool enabled() { return (setup_enabled_? setup_enabled_->value() : false); }

    bool alwaysShowAllSelectors(){return alwaysShowAllSelectors_;}
    bool dataAvailable() { return dataAvailable_; }

    virtual const char *userInit();

    virtual void userTerm() { return; }

    bool update() {
        beforeUpdate();
        bool result = doUpdate();
        if(result)
            afterUpdate();
        return result;
    }

    QString appName() { return appName_; }

    virtual int polling() {
        if(setup_enabled_ && (setup_enabled_)->value())
        {
            //qDebug() << "update - " << this->appName() << (setup_enabled_)->value() << " : next in " << pollingRate() << "ms";
            update();
            return pollingRate();
        }
        else
            return 2000;
    }

    bool needsRebuild() { return needsRebuild_; }
    void clearNeedsRebuild() { needsRebuild_ = false; }

    QStringList getTypes();

    QStringList getGroups(QString type);

    SensorItem getItem(QString type, QString group, QString item, bool *ok=0, int *selectedIndex=0);
    SensorGroup* getGroup(QString type, QString group, bool *ok=0);


    QStringList getItems(QString type, QString group, bool includeGroups);

    SensorType getType(QString name);

    QMetaType::Type getValueType(QString key);

    QVariantMap getMonitoringOptions();

signals:
    void availabiltyChanged();
};

#endif // LH_MONITORINGSOURCE_H
