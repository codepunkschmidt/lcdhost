/**
  \file     LH_MonitoringText.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese
    This module is based on original work by Johan Lindh and uses code freely
    available from http://allthingsgeek.wordpress.com/ by "Noccy"

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

#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QRegExp>

#include "LH_MonitoringText.h"

LH_MonitoringText::LH_MonitoringText(){
    data_ = NULL;
}

const char *LH_MonitoringText::userInit()
{
    if( const char *err = LH_Text::userInit() ) return err;
    LH_Qt_QString* hr = new LH_Qt_QString(this,tr("~Mon-Data-Rule"), QString(), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_htmlhelp );
    hr->setHelp("<hr>");
    hr->setOrder(-3);

    setup_value_round_ = new LH_Qt_bool(this,"Round",false, LH_FLAG_AUTORENDER);
    setup_value_round_->setHelp( "<p>Round non integer values.</p>");
    setup_value_round_->setOrder(-3);
    connect( setup_value_round_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_append_units_ = new LH_Qt_bool(this, "Append Units", true, 0);
    setup_append_units_->setHelp( "<p>Append the units to the text.</p>");
    setup_append_units_->setOrder(-3);
    connect( setup_append_units_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_pre_text_ = new LH_Qt_QString( this, "Pre-Text", "", LH_FLAG_AUTORENDER);
    setup_pre_text_->setHelp( "<p>Text to be displayed before the sensor value.</p>");
    setup_pre_text_->setOrder(-3);
    connect( setup_pre_text_, SIGNAL(changed()), this, SLOT(updateText()) );

    setup_post_text_ = new LH_Qt_QString(this, "Post-Text", "", LH_FLAG_AUTORENDER);
    setup_post_text_->setHelp( "<p>Text to be displayed after the sensor value.</p>");
    setup_post_text_->setOrder(-3);
    connect( setup_post_text_, SIGNAL(changed()), this, SLOT(updateText()) );

    LH_Qt_QString* hr2 =new LH_Qt_QString(this,tr("~Mon-Props-Rule"), QString(), LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK,lh_type_string_htmlhelp );
    hr2->setHelp("<hr>");
    hr2->setOrder(-3);

    setup_text_->setFlag( LH_FLAG_HIDDEN, true );
    setText(" ");
    return 0;
}

int LH_MonitoringText::notify(int n,void* p)
{
    if( !n || n&LH_NOTE_SECOND )
        updateText();

    return LH_Text::notify(n,p) | LH_NOTE_SECOND;
}

void LH_MonitoringText::updateText(bool rerender)
{
    if(data_)
    {
        QString valueString = data_->getText(setup_append_units_->value(), setup_value_round_->value());
        QString resultText = setup_pre_text_->value() + valueString + setup_post_text_->value();

        rerender = setText( resultText ) || rerender;

        if( rerender ) callback(lh_cb_render,NULL);
    }
}

void LH_MonitoringText::changeType()
{
    updateText();
}

void LH_MonitoringText::connect_updateText(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(updateText()) );
}

void LH_MonitoringText::connect_changeType(QObject* obj)
{
    connect( obj, SIGNAL(changed()), this, SLOT(changeType()) );
}
