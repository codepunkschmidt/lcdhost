#ifndef LH_MONITORINGSOURCES_H
#define LH_MONITORINGSOURCES_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QTimer>

#include "LH_MonitoringSource.h"
#include "json.h"

class LH_MonitoringSources: public LH_QtObject
{
    Q_OBJECT

    QHash<QString, LH_MonitoringSource*> sources_;
    QTimer* rebuildTimer_;

    LH_Qt_QString* setup_monitoring_options_;
public:
    LH_MonitoringSources(LH_QtObject *parent);

    virtual const char *userInit();
    virtual void userTerm();

    bool needsRebuild();
    void clearNeedsRebuild();
    bool rebuild();

    QStringList getApplications();

    LH_MonitoringSource* source(QString key) { return (sources_.contains(key)? sources_.value(key) : NULL); }

    bool add(LH_MonitoringSource* source);

    QString getLinkPath(QString app,QString type,QString group,QString item) { return (sources_.contains(app)?  sources_.value(app)->getLinkPath(type,group,item) : ""); }

    QString getOptionsLinkPath() { return QString("/%1/Options").arg(STRINGIZE(MONITORING_FOLDER)); }

    void refreshMonitoringOptions();

public slots:
    void doRebuild();

};


extern LH_MonitoringSources* dataSources;

#endif // LH_MONITORINGSOURCES_H
