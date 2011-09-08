/**
  \file     LH_CursorImage.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
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

#include "LH_CursorImage.h"

#include <QtGlobal>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QStringList>
#include <QString>
#include <QRegExp>
#include <QHash>

// #include <windows.h>
#include "LH_Qt_QStringList.h"

LH_PLUGIN_CLASS(LH_CursorImage)

lh_class *LH_CursorImage::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorImage",
        "Cursor Image",
        48,48


    };

    return &classInfo;
}

const char *LH_CursorImage::userInit()
{
    if( const char *err = LH_QImage::userInit() ) return err;

    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_AUTORENDER);
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
    setup_json_data_->refreshData();

    setup_cursor_state_ = new LH_Qt_QStringList( this, ("Cursor State"), QStringList()<<"OFF"<<"OFF_SEL"<<"ON"<<"ON_SEL", LH_FLAG_NOSAVE|LH_FLAG_NOSINK|LH_FLAG_NOSOURCE|LH_FLAG_READONLY );

    connect(setup_json_data_,SIGNAL(changed()),this,SLOT(updateState()));

    add_cf_target(setup_image_file_);
    add_cf_source(setup_cursor_state_);

    return 0;
}

bool LH_CursorImage::updateState()
{
    bool newSelected;
    bool newActive;
    QString newStatusCode = cursorData(setup_json_data_->value()).getState(setup_coordinate_->value().split(';'),newSelected,newActive);

    if(setup_cursor_state_->value() != newStatusCode)
    {
        setup_cursor_state_->setValue(newStatusCode);
        return true;
    }
    else
        return false;
}
