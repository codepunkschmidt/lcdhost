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

LH_MonitoringGraph::LH_MonitoringGraph( const char *name, LH_QtPlugin *parent = 0 ) : LH_Graph( name, parent, 0, 2 )
{
    setUserDefinableLimits(true);
    canGrow(true);

    setup_auto_scale_y_max_->setValue(true);
    setup_auto_scale_y_min_->setValue(true);
    setup_show_y_max_->setValue(true);
    setup_show_y_min_->setValue(true);

    setup_append_units_ = new LH_Qt_bool(this, "Append Units", true, 0);
    setup_append_units_->setHelp( "<p>Append the units to the text.</p>");
    setup_append_units_->setOrder(-3);
    connect( setup_append_units_, SIGNAL(changed()), this, SLOT(updateUnits()) );

    was_empty_ = true;
}

LH_MonitoringGraph::~LH_MonitoringGraph()
{

}

int LH_MonitoringGraph::notify(int n, void *p)
{
    if(!n || n&LH_NOTE_SECOND)
        if(data_)
        {
            float deadVal;
            bool hasDead = (data_->getDeadValue_Transformed(deadVal));

            int count;
            data_->getCount(count);
            if(lineCount() != count)
                updateLines();

            if(!data_->isGroup())
            {
                float currVal=0;
                data_->getValue(currVal);
                addValue(currVal);
            } else {
                if(hasDead) setDeadValue((qreal)deadVal);

                for(int i=0; i<lineCount(); i++)
                {
                    float currVal=0;
                    int index = data_->lookupIndex(i);
                    data_->getValue(currVal, index);
                    addValue(currVal, i);
                }
            }
            if(was_empty_ && !graph_empty_) updateUnits();
            was_empty_ = graph_empty_;
            callback(lh_cb_render,NULL);
        }
    return LH_NOTE_SECOND;
}

void LH_MonitoringGraph::updateLines()
{
    QStringList names;
    if(data_)
        data_->getNames(names);
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
    if(data_)
        if(setup_append_units_->value()) units = data_->getUnits();
    setYUnit(units);
}

void LH_MonitoringGraph::connect_clearData(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(clearData()) );
}
