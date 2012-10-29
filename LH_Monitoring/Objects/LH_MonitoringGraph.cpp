/**
  \file     LH_MonitoringGraph.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

  */

#include "LH_MonitoringGraph.h"

#include <QDebug>

LH_PLUGIN_CLASS(LH_MonitoringGraph)

lh_class *LH_MonitoringGraph::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        STRINGIZE(MONITORING_FOLDER),
        STRINGIZE(COMMON_OBJECT_NAME)"Graph",
        STRINGIZE(COMMON_OBJECT_NAME)" (Graph)",
        48,48

    };

    return &classInfo;
}

LH_MonitoringGraph::LH_MonitoringGraph() :
    LH_Graph(0, 2, gdmExternallyManaged, NULL),
    LH_MonitoringObject(this, mdmNumbers, true, false),
    was_empty_(true),
    setup_append_units_(0)
{
    monitoringInit(SLOT(refreshMonitoringOptions()),
                   SLOT(connectChangeEvents()),
                   SLOT(changeAppSelection()),
                   SLOT(changeTypeSelection()),
                   SLOT(changeGroupSelection()),
                   SLOT(changeItemSelection()),
                   SLOT(dataValidityChanged()));
}

const char *LH_MonitoringGraph::userInit()
{
    if( const char *err = LH_Graph::userInit() ) return err;

    //this->LH_Text::connect( setup_value_str_, SIGNAL(changed()), this, SLOT(updateText()) );
    //this->LH_Text::connect( setup_value_str_, SIGNAL(set()), this, SLOT(updateText()) );

    setUserDefinableLimits(true);
    //canGrow(true);

    setup_auto_scale_y_max_->setValue(true);
    setup_auto_scale_y_min_->setValue(true);
    setup_show_y_max_->setValue(true);
    setup_show_y_min_->setValue(true);

    setup_append_units_ = new LH_Qt_bool(this, "Append Units", true, 0);
    setup_append_units_->setHelp( "<p>Append the units to the text.</p>");
    setup_append_units_->setOrder(-3);
    connect( setup_append_units_, SIGNAL(changed()), this, SLOT(updateUnits()) );

    (new LH_Qt_QString(this,("image-hr2"), QString("<hr>"), LH_FLAG_NOSAVE_LINK | LH_FLAG_NOSAVE_DATA | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDETITLE,lh_type_string_html ))->setOrder(-3);

    was_empty_ = true;

    connect(this, SIGNAL(initialized()), SLOT(doInitialize()) );

    return 0;
}

void LH_MonitoringGraph::doInitialize()
{
    //connect(ui_->setup_unit_selection_, SIGNAL(changed()), SLOT(clearData()) );

    if(canGrow())
        LH_Graph::clear(min_val(), min_val()+1, canGrow());
    else
        LH_Graph::clear(min_val(), max_val(), canGrow());
    updateDataCache();
    updateUnits();
}

int LH_MonitoringGraph::notify(int n, void *p)
{
    Q_UNUSED(p);

    if(!n || n&LH_NOTE_SECOND)
        callback(lh_cb_render,NULL);
    /*
        if(ui_ && ui_->data_)
        {
            float deadVal;
            bool hasDead = (ui_->data_->getDeadValue_Transformed(deadVal));

            int count;
            ui_->data_->getCount(count);
            if(lineCount() != count)
                updateLines();

            if(!ui_->data_->isGroup())
            {
                float currVal=0;
                ui_->data_->getValue(currVal);
                addValue(currVal);
            } else {
                if(hasDead) setDeadValue((qreal)deadVal);

                for(int i=0; i<lineCount(); i++)
                {
                    float currVal=0;
                    int index = ui_->data_->lookupIndex(i);
                    ui_->data_->getValue(currVal, index);
                    addValue(currVal, i);
                }
            }
            if(was_empty_ && !graph_empty_) updateUnits();
            was_empty_ = graph_empty_;
            callback(lh_cb_render,NULL);
        }*/
    return LH_QtInstance::notify(n,p) | LH_NOTE_SECOND;
}

void LH_MonitoringGraph::updateLines()
{
    QStringList names;
    bool ok;
    getValuesVector(false, 0, ok, &names);
    if(ok)
        setLines(names);
}

QImage *LH_MonitoringGraph::render_qimage( int w, int h )
{
    if( LH_Graph::render_qimage(w,h) == NULL ) return NULL;

    if(setup_value_ptr_->value()==(int)(externalSource()))
        drawAll();

    return image_;
}

void LH_MonitoringGraph::configChanged()  {
    clearData();
    updateUnits();
}

void LH_MonitoringGraph::clearData()
{
    LH_Graph::clear(min_val(), max_val(), canGrow());
    updateUnits();
    callback(lh_cb_render,NULL);
}

void LH_MonitoringGraph::updateUnits()
{
    QString units = "";
    if(setup_append_units_->value())
    {
        bool ok;
        SensorItem item = selectedSensor(&ok);
        if(ok)
            units = item.units;
    }
    setYUnit(units);
}

void LH_MonitoringGraph::updateDataCache()
{
    bool ok1;
    bool ok2;
    int selectedIndex;

    SensorGroup* group = selectedSensorGroup(&ok1);
    SensorItem item = selectedSensor(&ok2, &selectedIndex);

    if(ok1 && ok2)
    {
        LH_Graph::setExternalSource(&(group->items));
        clearLines();
        for(int i=0; i<group->items.count(); i++)
        {
            if(item.group)
                group->items[i].hidden = (group->items[i].aggregate || group->items[i].group);
            else
                group->items[i].hidden = (i!=selectedIndex);

            if(!group->items[i].hidden)
                addLine(group->items[i].name);
        }
    }
    else
    {
        LH_Graph::setExternalSource(NULL);
        clearLines();
    }
}
