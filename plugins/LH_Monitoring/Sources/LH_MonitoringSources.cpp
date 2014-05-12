#include "LH_MonitoringSources.h"
#include "../LH_QtMonitoringPlugin.h"

LH_MonitoringSources* getDataSources()
{
    if (LH_QtMonitoringPlugin* plugin = qobject_cast<LH_QtMonitoringPlugin*>(lh_plugin())) {
        return plugin->dataSources();
    }
    return 0;
}

LH_MonitoringSources::LH_MonitoringSources(LH_QtObject *parent): LH_QtObject(parent), sources_(), setup_monitoring_options_(0)
{   
    rebuildTimer_ = new QTimer(this);
    rebuildTimer_->setInterval(1000);
    connect(rebuildTimer_, SIGNAL(timeout()), this, SLOT(doRebuild()));
}

const char *LH_MonitoringSources::userInit() {
    setup_monitoring_options_ = new LH_Qt_QString(this->parent(), "Options", "", LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_NOSAVE_LINK | LH_FLAG_NOSAVE_DATA | LH_FLAG_NOSINK );
    setup_monitoring_options_->setPublishPath(getOptionsLinkPath());
    setup_monitoring_options_->setMimeType("text/json");

    foreach(LH_MonitoringSource* source, sources_)
        if(const char* r = source->userInit()) return r;

    refreshMonitoringOptions();
    return 0;
}

void LH_MonitoringSources::userTerm() {
    foreach(LH_MonitoringSource* source, sources_)
        source->userTerm();
}

bool LH_MonitoringSources::needsRebuild() {
    bool _needsRebuild = false;
    foreach(LH_MonitoringSource* source, sources_)
        _needsRebuild |= source->needsRebuild();
    return _needsRebuild;
}

void LH_MonitoringSources::clearNeedsRebuild() {
    foreach(LH_MonitoringSource* source, sources_)
        source->clearNeedsRebuild();
}

void LH_MonitoringSources::doRebuild()
{
    rebuildTimer_->stop();
    this->parent()->requestRebuild();
}

bool LH_MonitoringSources::rebuild()
{
    if (needsRebuild())
    {
        clearNeedsRebuild();
        rebuildTimer_->start();
        return true;
    }
    else
        return false;
}

QStringList LH_MonitoringSources::getApplications()
{
    QStringList l;
    foreach(LH_MonitoringSource* source, sources_) {
        if(source->enabled())
            l.append(source->appName());
    }
    l.sort();
    if( sources_.count() >= 2 )
        l.insert(0,"(Please Select)");
    return l;
}

bool LH_MonitoringSources::add(LH_MonitoringSource* source)
{
    if(sources_.contains(source->appName()))
        return false;
    else
    {
        sources_.insert(source->appName(), source);
        return true;
    }
}

void LH_MonitoringSources::refreshMonitoringOptions()
{
    QVariantMap options;

    foreach(LH_MonitoringSource* source, sources_)
        if(source->enabled())
            options.insert(source->appName(),source->getMonitoringOptions());

    QString jsonData = QString::fromUtf8(Json::serialize(options));
    setup_monitoring_options_->setValue(jsonData);
}
