/**
  \file     LH_CursorPage.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010,2011 Andy Bridges
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

#include "LH_CursorPage.h"
#include <QtGlobal>
#include <QDebug>
#include <QSharedMemory>

LH_PLUGIN_CLASS(LH_CursorPage)

lh_class *LH_CursorPage::classInfo()
{
    static lh_class classinfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorPage",
        "Cursor Page",
        -1, -1
    };
    return &classinfo;
}

LH_CursorPage::LH_CursorPage()
{
    active = false;
    selected = false;
}

const char *LH_CursorPage::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;

    setVisible(selected);

    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_AUTORENDER);
    setup_coordinate_->setHelp("This is the coordinate of this object, i.e. when the cursor is at the point specified here this object is selected. <br/>"
                               "<br/>"
                               "Note that many objects can have the same coordinate if the user requires.<br/>"
                               "<br/>"
                               "The format is [x],[y] <br/>"
                               "e.g.: 1,1<br/>"
                               "<br/>"
                               "When selected a page (and all its children) is visible. When not selected it (and all its children) are hidden, although they can still be selected in the Instances tree."
                               );
    setup_json_data_ = new LH_Qt_QString(this, "Cursor Data", "", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_LAST /*| LH_FLAG_READONLY | LH_FLAG_HIDEVALUE*/);
    setup_json_data_->setLink("Cursors/#1");
    setup_json_data_->refreshData();

    connect(setup_json_data_,SIGNAL(changed()),this,SLOT(updateState()));
    return 0;
}

QImage *LH_CursorPage::render_qimage( int w, int h )
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    return image_;
}


bool LH_CursorPage::updateState()
{
    QStringList mycoords = setup_coordinate_->value().split(';');
    cursorData cursor_data(setup_json_data_->value());

    bool newSelected = false;
    bool newActive = false;
    foreach (QString mycoord_str, mycoords)
    {
        QStringList mycoord = mycoord_str.split(',');
        if(mycoord.length()==2)
        {
            int myX = mycoord.at(0).toInt();
            int myY = mycoord.at(1).toInt();

            newSelected |= ( cursor_data.selState && cursor_data.selX==myX && cursor_data.selY==myY );
            newActive |= ( cursor_data.active && cursor_data.x==myX && cursor_data.y==myY );
        }
    }
    if(selected!=newSelected || active != newActive)
    {
        selected = newSelected;
        setVisible(selected);
        active = newActive;
        return true;
    }
    else
        return false;
}

