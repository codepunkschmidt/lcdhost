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

LH_PLUGIN_CLASS(LH_CursorRectangle)

lh_class *LH_CursorRectangle::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorRectangle",
        "Cursor Rectangle",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_CursorRectangle::LH_CursorRectangle( const char *name ) : LH_QtInstance(name,0, 0)
{
    setup_penwidth_ = new LH_Qt_QSlider(this,tr("Pen width"),0,0,1000,LH_FLAG_AUTORENDER);
    setup_rounding_ = new LH_Qt_QSlider(this,tr("Corner rounding"),20,0,100,LH_FLAG_AUTORENDER);
    setup_pencolor_ = new LH_Qt_QColor(this,tr("Pen color"),Qt::black,LH_FLAG_AUTORENDER);

    setup_file_ = new LH_Qt_QFileInfo( this, tr("Color Map"), QFileInfo(), LH_FLAG_AUTORENDER );
    setup_file_->setHelp( "<p>The color map file instructs the status image on how to match up "
                          "a state with the colors & gradients.</p>"
                          "<p>There are four states for Cursor Images:"
                          "<ul>"
                          "<li><b>OFF</b> (The cursor is not over this item, nor is it selected)</li>"
                          "<li><b>ON</b> (The cursor is over this item, but it is <i>not</i> selected)</li>"
                          "<li><b>OFF_SEL</b> (The cursor is not over this item, but it <i>is</i> selected)</li>"
                          "<li><b>ON_SEL</b> (The cursor is over this item, and it <i>is</i> selected)</li>"
                          "</ul>"
                          "</p>"
                          "<p>The format is as follows:</p>"
                          "<p><nobr>&lt;Code&gt;	&lt;Color1&gt;	&lt;Alpha&gt;	&lt;Color2&gt;	&lt;Alpha&gt;	&lt;Gradient Direction (Horizontal or Vertical)&gt;</nobr></p>"
                          "<p>Note that each item is seperated by a tab, and that the second color and gradient direction are optional (vertical gradients are the default; horizontal gradients can be defined with either h, horiz, or horizontal).</p>"
                          "<p>e.g.<br/>"
                          "OFF	white	255<br/>"
                          "ON	#9ca0bc	255	#d4d9ff	255 horiz<br/>"
                          "</p>");
    connect( setup_file_, SIGNAL(changed()), this, SLOT(fileChanged()) );
    setup_text_ = new LH_Qt_QString( this, tr("~"), QString(), LH_FLAG_READONLY|LH_FLAG_NOSAVE|LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER );

    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_AUTORENDER);
    setup_coordinate_->setHelp("This is the coordinate of this object, i.e. when the cursor is at the point specified here this object is selected. <br/>"
                               "<br/>"
                               "Note that many objects can have the same coordinate if the user requires.<br/>"
                               "<br/>"
                               "The format is [x],[y] <br/>"
                               "e.g.: 1,1"
                               );

    setup_layout_trigger_ = new LH_Qt_bool(this,"Layout Trigger",false,0);
    setup_layout_trigger_->setHelp("Cursor Rectangles can be used to create a simple menu. Simply check this box and when selected the selected layout will be opened.<br/>"
                                   "<br/>"
                                   "<b>Layout Designers Beware!</b> firing off a \"load layout\" command will not give you the option of saving any changes you may have made to the current layout! Make sure you haved saved your layout before testing layout loading!");
    setup_layout_ = new LH_Qt_QFileInfo(this, "Layout", QFileInfo(), LH_FLAG_HIDDEN);
    setup_layout_trigger_->setHelp("The layout to load when the rectangle is selected.");

    connect(setup_layout_trigger_, SIGNAL(changed()), this, SLOT(changeLayoutTrigger()));

    statusCode_ = "OFF";
    colorDefinitions.insert("OFF",     (colorMapData){"OFF",     QColor("white"),  false, QColor(0,0,0,0), false});
    colorDefinitions.insert("ON",      (colorMapData){"ON",      QColor("red"),    false, QColor(0,0,0,0), false});
    colorDefinitions.insert("OFF_SEL", (colorMapData){"OFF_SEL", QColor("blue"),   false, QColor(0,0,0,0), false});
    colorDefinitions.insert("ON_SEL",  (colorMapData){"ON_SEL",  QColor("purple"), false, QColor(0,0,0,0), false});

    //Color overrides:
    {
        setup_color_overrides_ = new LH_Qt_bool(this,tr("^Use Color Map Overrides"),false,LH_FLAG_AUTORENDER);
        setup_color_overrides_->setHelp("Checking this option will allow you to override any colors defined in the color map.");

        setup_override_off_ = new LH_Qt_bool(this,tr("Override Colors\n(Default)"),false,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor1_off_ = new LH_Qt_QColor(this,tr("Fill color 1\n(Default)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor2_off_ = new LH_Qt_QColor(this,tr("Fill color 2\n(Default)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);

        setup_override_on_ = new LH_Qt_bool(this,tr("Override Colors\n(Highlight)"),false,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor1_on_ = new LH_Qt_QColor(this,tr("Fill color 1\n(Highlight)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor2_on_ = new LH_Qt_QColor(this,tr("Fill color 2\n(Highlight)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);

        setup_override_off_sel_ = new LH_Qt_bool(this,tr("Override Colors\n(Selected)"),false,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor1_off_sel_ = new LH_Qt_QColor(this,tr("Fill color 1\n(Selected)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor2_off_sel_ = new LH_Qt_QColor(this,tr("Fill color 2\n(Selected)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);

        setup_override_on_sel_ = new LH_Qt_bool(this,tr("Override Colors\n(Active & Highlight)"),false,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor1_on_sel_ = new LH_Qt_QColor(this,tr("Fill color 1\n(Active & Highlight)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);
        setup_bgcolor2_on_sel_ = new LH_Qt_QColor(this,tr("Fill color 2\n(Active & Highlight)"),Qt::transparent,LH_FLAG_HIDDEN|LH_FLAG_AUTORENDER);

        //setup_gradient_ = new LH_Qt_bool(this,tr("^Background is a gradient"),false,LH_FLAG_HIDDEN);
        //setup_horizontal_ = new LH_Qt_bool(this,tr("^Gradient is horizontal"),false,LH_FLAG_HIDDEN);

        connect(setup_color_overrides_, SIGNAL(changed()), this, SLOT(changeColorOverrides()));

        connect(setup_override_off_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor1_off_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor2_off_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_override_on_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor1_on_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor2_on_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_override_off_sel_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor1_off_sel_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor2_off_sel_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_override_on_sel_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor1_on_sel_, SIGNAL(changed()), this, SLOT(overridesChanged()));
        connect(setup_bgcolor2_on_sel_, SIGNAL(changed()), this, SLOT(overridesChanged()));
    }

}

QImage *LH_CursorRectangle::render_qimage( int w, int h )
{
    qreal minside;
    qreal rounding;
    qreal penpixels;

    if( LH_QtInstance::initImage(w,h) == NULL ) return NULL;
    image_->fill( qRgba(0,0,0,0) );

    minside = qMin( image_->width(), image_->height() ) / 2;
    rounding = ( minside * (qreal) this->rounding() ) / 100.0;

    QPainter painter;
    if( painter.begin( image_ ) )
    {
        QRectF rect = image_->rect();

        painter.setRenderHint( QPainter::Antialiasing, true );

        if( penwidth() )
        {
            QPen pen( pencolor() );
            penpixels = (minside * (qreal) penwidth()) / 1000.0;
            pen.setWidthF( penpixels );
            rect.adjust(+penpixels/2,+penpixels/2,-penpixels/2,-penpixels/2);
            painter.setPen( pen );
        }
        else
        {
            painter.setPen( Qt::NoPen );
            penpixels = 0.0;
        }

        if( gradient() )
        {
            QLinearGradient gradient;
            gradient.setStart(0,0);
            if( horizontal() ) gradient.setFinalStop(image_->width(),0);
            else gradient.setFinalStop(0,image_->height());
            gradient.setColorAt(0, bgcolor1() );
            gradient.setColorAt(1, bgcolor2() );
            QBrush brush(gradient);
            painter.setBrush( brush );
        }
        else
            painter.setBrush( bgcolor1() );

        // There's a bug in Qt 4.5.3 which makes drawRoundedRect fail
        // in the upper-left quadrant. That quadrant becomes too 'small'
        // compared to the others. The simple workaround is to use a
        // QPainterPath with 'addRoundedRect' and then just draw
        // that path instead.
        // see http://bugreports.qt.nokia.com/browse/QTBUG-6073

        // painter.drawRoundedRect( rect, rounding, rounding, Qt::AbsoluteSize );

        QPainterPath path;
        path.addRoundedRect( rect, rounding, rounding, Qt::AbsoluteSize );
        painter.drawPath( path );

        rounding += penpixels;
        qreal dx = (rect.left() + 0.31 * rounding);
        qreal dy = (rect.top() + 0.31 * rounding);
        if( dx >= image_->width()/2 ) dx = (image_->width()/2)-1;
        if( dy >= image_->height()/2 ) dy = (image_->height()/2)-1;
        if( dx < 0 ) dx = 0;
        if( dy < 0 ) dy = 0;

        painter.end();
    }

    return image_;
}

int LH_CursorRectangle::polling()
{
    if(updateState()) callback(lh_cb_render,NULL);
    return 100;
}

bool LH_CursorRectangle::updateState()
{
    QStringList mycoords = setup_coordinate_->value().split(';');

    bool newSelected = false;
    bool newActive = false;
    foreach (QString mycoord_str, mycoords)
    {
        QStringList mycoord = mycoord_str.split(',');
        if(mycoord.length()==2)
        {
            int myX = mycoord.at(0).toInt();
            int myY = mycoord.at(1).toInt();

            newSelected = newSelected || ( cursor_data.selState && cursor_data.selX==myX && cursor_data.selY==myY );
            newActive = newActive ||  ( cursor_data.active && cursor_data.x==myX && cursor_data.y==myY );
        }
    }

    QString newStatusCode = QString("%1%2").arg(newActive? "ON" : "OFF").arg(newSelected? "_SEL" : "");
    if(statusCode_ != newStatusCode)
    {
        statusCode_ = newStatusCode;

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

bool LH_CursorRectangle::gradient(){
    if(colorDefinitions.contains(statusCode_))
        return colorDefinitions.value(statusCode_).isGradient;
    else
        return false;
}
bool LH_CursorRectangle::horizontal() {
    if(colorDefinitions.contains(statusCode_))
        return colorDefinitions.value(statusCode_).isHorizontal;
    else
        return false;
}

QColor LH_CursorRectangle::bgcolor1()
{
    if(colorDefinitions.contains(statusCode_))
        return colorDefinitions.value(statusCode_).color1;
    else
        return QColor(255,255,255,255);
}

QColor LH_CursorRectangle::bgcolor2()
{
    if(colorDefinitions.contains(statusCode_))
        return colorDefinitions.value(statusCode_).color2;
    else
        return QColor(0,0,0,255);
}

void LH_CursorRectangle::changeLayoutTrigger()
{
    setup_layout_->setFlag(LH_FLAG_HIDDEN, !setup_layout_trigger_->value());
}

void LH_CursorRectangle::fileChanged()
{
    setup_file_->value().refresh();
    if( !setup_file_->value().isFile() )
    {
        setup_text_->setValue(tr("No such file."));
        setup_text_->setFlag(LH_FLAG_HIDDEN,false);
        applyOverrides();
        return;
    }
    else
    {
        setup_text_->setFlag(LH_FLAG_HIDDEN,true);
        QFile file( setup_file_->value().filePath() );

        if( file.open( QIODevice::ReadOnly) )
        {
            QTextStream stream(&file);
            QRegExp re = QRegExp(";.*$");
            QString fileContent = stream.readAll();

            QStringList items = fileContent.split('\r',QString::SkipEmptyParts);

            foreach (QString item, items)
            {
                item = item.remove(re).trimmed();
                if (item!="")
                {
                    QStringList parts = item.split('\t',QString::SkipEmptyParts);
                    colorMapData cm = {parts[0], QColor(0,0,0,0), false, QColor(0,0,0,0), false};
                    if (parts.length()>1) cm.color1 = QColor(parts[1]);
                    if (parts.length()>2) cm.color1.setAlpha(parts[2].toInt());
                    cm.isGradient = (parts.length()>3);
                    if (parts.length()>3) cm.color2 = QColor(parts[3]);
                    if (parts.length()>4) cm.color2.setAlpha(parts[4].toInt());
                    if (parts.length()>5) cm.isHorizontal = QString("horizontal").startsWith(parts[5]);
                    colorDefinitions[cm.code] = cm;
                }
            }
            applyOverrides();
            callback(lh_cb_render,NULL);
        } else {
            setup_text_->setValue(tr("Unable to open file."));
            setup_text_->setFlag(LH_FLAG_HIDDEN,false);
            return;
        }
    }
}

void LH_CursorRectangle::changeColorOverrides()
{
    overridesChanged();

    setup_override_off_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor1_off_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor2_off_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());

    setup_override_on_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor1_on_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor2_on_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());

    setup_override_off_sel_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor1_off_sel_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor2_off_sel_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());

    setup_override_on_sel_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor1_on_sel_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    setup_bgcolor2_on_sel_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());

    //setup_gradient_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
    //setup_horizontal_->setFlag(LH_FLAG_HIDDEN, !setup_color_overrides_->value());
}

void LH_CursorRectangle::overridesChanged()
{
    fileChanged();
}

void LH_CursorRectangle::applyOverrides()
{
    if(setup_color_overrides_->value())
    {
        applyOverrides("OFF",     setup_override_off_, setup_bgcolor1_off_, setup_bgcolor2_off_);
        applyOverrides("ON",      setup_override_on_, setup_bgcolor1_on_, setup_bgcolor2_on_);
        applyOverrides("OFF_SEL", setup_override_off_sel_, setup_bgcolor1_off_sel_, setup_bgcolor2_off_sel_);
        applyOverrides("ON_SEL",  setup_override_on_sel_, setup_bgcolor1_on_sel_, setup_bgcolor2_on_sel_);
    }
}
void LH_CursorRectangle::applyOverrides(QString testState, LH_Qt_bool* testOverride, LH_Qt_QColor* testCol1, LH_Qt_QColor* testCol2)
{
    if(testOverride->value())
    {
        colorDefinitions[testState].color1 = testCol1->value();
        colorDefinitions[testState].color2 = testCol2->value();
        colorDefinitions[testState].isGradient = (testCol1->value() != testCol2->value());
    }
}
