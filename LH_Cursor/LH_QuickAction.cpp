/**
  \file     LH_QuickAction.cpp
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

#include "LH_QuickAction.h"

LH_PLUGIN_CLASS(LH_QuickAction)

lh_class *LH_QuickAction::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "QuickAction",
        "Quick Action",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_QuickAction::LH_QuickAction(const char * name, LH_QtPlugin *parent) : LH_QtInstance(name, 0, parent)
{
    setup_fire_ = new LH_Qt_InputState(this,"Trigger key","",0);
    setup_fire_ ->setHelp("Binding a key to this object will allow LCDHost to load a specified layout when the key is pressed, e.g. binding the G19's \"Menu\" key and configuring it to load a menu layout.");
    setup_layout_ = new LH_Qt_QFileInfo(this, "Layout", QFileInfo(), 0);
    setup_layout_->setHelp("The layout to load when the associated key is pressed.<br/>"
                           "<br/>"
                           "<b>Layout Designers Beware!</b> firing off a \"load layout\" command will not give you the option of saving any changes you may have made to the current layout! Make sure you haved saved your layout before testing layout loading!");

    connect( setup_fire_, SIGNAL(input(QString,int,int)), this, SLOT(doFire(QString,int,int)) );
}

void LH_QuickAction::doFire(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);

    if(setup_layout_->value().isFile())
    {
        static QByteArray ary;
        ary = setup_layout_->value().absoluteFilePath().toUtf8();
        callback(lh_cb_load_layout, ary.data() );
    }
}
