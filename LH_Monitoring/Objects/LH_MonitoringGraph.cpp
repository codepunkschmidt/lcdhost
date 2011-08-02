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

LH_PLUGIN_CLASS(LH_MonitoringGraph)

lh_class *LH_MonitoringGraph::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "3rdParty/Monitoring",
        "MonitoringGraph",
        "Monitoring (Graph)",
        48,48
        
    };

    return &classInfo;
}

const char *LH_MonitoringGraph::userInit()
{
    if( const char *err = LH_Graph::userInit() ) return err;
    ui_ = new LH_MonitoringUI(this, mdmNumbers, true);

    setUserDefinableLimits(true);
    canGrow(true);

    LH_Qt_QString* hr = new LH_Qt_QString(this,("Mon-Data-Rule"), QString(), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDETITLE,lh_type_string_htmlhelp );
    hr->setHelp("<hr>");
    hr->setOrder(-3);

    setup_auto_scale_y_max_->setValue(true);
    setup_auto_scale_y_min_->setValue(true);
    setup_show_y_max_->setValue(true);
    setup_show_y_min_->setValue(true);

    setup_append_units_ = new LH_Qt_bool(this, "Append Units", true, 0);
    setup_append_units_->setHelp( "<p>Append the units to the text.</p>");
    setup_append_units_->setOrder(-3);
    connect( setup_append_units_, SIGNAL(changed()), this, SLOT(updateUnits()) );

    LH_Qt_QString* hr2 =new LH_Qt_QString(this,("Mon-Props-Rule"), QString(), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_HIDETITLE,lh_type_string_htmlhelp );
    hr2->setHelp("<hr>");
    hr2->setOrder(-3);

    was_empty_ = true;

    connect(ui_, SIGNAL(appChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(typeChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(groupChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(itemChanged()), this, SLOT(configChanged()) );
    connect(ui_, SIGNAL(initialized()), this, SLOT(configChanged()) );

    return 0;
}

int LH_MonitoringGraph::notify(int n, void *p)
{
    Q_UNUSED(p);

    if(!n || n&LH_NOTE_SECOND)
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
        }
    return LH_Graph::notify(n,p) | LH_NOTE_SECOND;
}

void LH_MonitoringGraph::updateLines()
{
    QStringList names;
    if(ui_ && ui_->data_)
        ui_->data_->getNames(names);
    setLines(names);
}

QImage *LH_MonitoringGraph::render_qimage( int w, int h )
{
    if( LH_Graph::render_qimage(w,h) == NULL ) return NULL;
    drawAll();
    return image_;
}

void LH_MonitoringGraph::clearData()
{
    clear();
    updateUnits();
    callback(lh_cb_render,NULL);
}

void LH_MonitoringGraph::updateUnits()
{
    QString units = "";
    if(ui_ && ui_->data_)
        if(setup_append_units_->value()) units = ui_->data_->getUnits();
    setYUnit(units);
}
