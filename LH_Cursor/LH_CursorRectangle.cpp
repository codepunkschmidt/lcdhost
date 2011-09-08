/**
  \file     LH_CursorRectangle.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2010,2011 Andy Bridges & Johan Lindh
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
**/

#include <QDebug>
#include <QPainter>

#include "LH_CursorRectangle.h"
#include "LH_Qt_html.h"

LH_PLUGIN_CLASS(LH_CursorRectangle)

lh_class *LH_CursorRectangle::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorRectangle",
        "Cursor Rectangle",
        48,48
    };

    return &classInfo;
}

const char *LH_CursorRectangle::userInit()
{
    if( const char *err = LH_Rectangle::userInit() ) return err;

    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_AUTORENDER | LH_FLAG_FIRST);
    setup_coordinate_->setHelp("This is the coordinate of this object, i.e. when the cursor is at the point specified here this object is selected. <br/>"
                               "<br/>"
                               "Note that many objects can have the same coordinate if the user requires.<br/>"
                               "<br/>"
                               "The format is [x],[y] <br/>"
                               "e.g.: 1,1"
                               );

    setup_json_data_ = new LH_Qt_QString(this, "Cursor Data", "", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_LAST /*| LH_FLAG_READONLY | LH_FLAG_HIDEVALUE*/);
    setup_json_data_->setLink("Cursors/Primary Cursor");
    setup_json_data_->setLinkFilter("Cursors");
    setup_json_data_->refreshValue();

    setup_cursor_state_ = new LH_Qt_QStringList( this, ("Cursor State"), QStringList()<<"OFF"<<"OFF_SEL"<<"ON"<<"ON_SEL", LH_FLAG_NOSAVE|LH_FLAG_NOSINK|LH_FLAG_NOSOURCE|LH_FLAG_READONLY | LH_FLAG_FIRST );

    new LH_Qt_html(this,"<hr>", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK | LH_FLAG_FIRST );
    new LH_Qt_html(this,"<hr>", LH_FLAG_NOSOURCE | LH_FLAG_NOSINK );

    setup_layout_trigger_ = new LH_Qt_bool(this,"Layout Trigger",false,0);
    setup_layout_trigger_->setHelp("Cursor Rectangles can be used to create a simple menu. Simply check this box and when selected the selected layout will be opened.<br/>"
                                   "<br/>"
                                   "<b>Layout Designers Beware!</b> firing off a \"load layout\" command will not give you the option of saving any changes you may have made to the current layout! Make sure you haved saved your layout before testing layout loading!");
    setup_layout_ = new LH_Qt_QFileInfo(this, "Layout", QFileInfo(), LH_FLAG_HIDDEN);
    setup_layout_trigger_->setHelp("The layout to load when the rectangle is selected.");

    connect(setup_json_data_,SIGNAL(changed()),this,SLOT(updateState()));
    connect(setup_layout_trigger_, SIGNAL(changed()), this, SLOT(changeLayoutTrigger()));

#ifdef LH_CF
    add_cf_target(setup_penwidth_);
    add_cf_target(setup_pencolor_);
    add_cf_target(setup_bgcolor1_);
    add_cf_target(setup_bgcolor2_);
    add_cf_target(setup_gradient_);
    add_cf_target(setup_horizontal_);
    add_cf_source(setup_cursor_state_);

    cf_set_rules("<rules>"
                 "<rule>"
                 "<conditions><condition test='Equals'><source>Cursor State</source><value id='1'>OFF</value></condition></conditions>"
                 "<actions><action type='property'><target>Fill color 1</target><value>FFFFFFFF</value></action></actions>"
                 "</rule>"
                 "<rule>"
                 "<conditions><condition test='Equals'><source>Cursor State</source><value id='1'>OFF_SEL</value></condition></conditions>"
                 "<actions><action type='property'><target>Fill color 1</target><value>FF0000FF</value></action></actions>"
                 "</rule>"
                 "<rule>"
                 "<conditions><condition test='Equals'><source>Cursor State</source><value id='1'>ON</value></condition></conditions>"
                 "<actions><action type='property'><target>Fill color 1</target><value>FFFF0000</value></action></actions>"
                 "</rule>"
                 "<rule>"
                 "<conditions><condition test='Equals'><source>Cursor State</source><value id='1'>ON_SEL</value></condition></conditions>"
                 "<actions><action type='property'><target>Fill color 1</target><value>FF800080</value></action></actions>"
                 "</rule>"
                 "</rules>");
#endif
    return 0;
}

bool LH_CursorRectangle::updateState()
{
    bool newSelected;
    bool newActive;
    QString newStatusCode = cursorData(setup_json_data_->value()).getState(setup_coordinate_->value().split(';'),newSelected,newActive);

    if(setup_cursor_state_->value() != newStatusCode)
    {
        setup_cursor_state_->setValue(newStatusCode);

        if(newSelected && setup_layout_trigger_->value() && setup_layout_->value().isFile())
        {
            static QByteArray ary;
            ary = setup_layout_->value().absoluteFilePath().toUtf8();
            callback(lh_cb_load_layout, ary.data() );
        }
        return true;
    }
    else
        return false;
}

void LH_CursorRectangle::changeLayoutTrigger()
{
    setup_layout_->setHidden( !setup_layout_trigger_->value());
}
