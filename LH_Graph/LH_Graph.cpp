/**
  \file     LH_Graph.cpp
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

#include <QtGlobal>
#include <QDebug>
#include <QLinearGradient>
#include <QPainter>

#include "LH_Graph.h"
#include <math.h>

static inline uint PREMUL(uint x) {
    uint a = x >> 24;
    uint t = (x & 0xff00ff) * a;
    t = (t + ((t >> 8) & 0xff00ff) + 0x800080) >> 8;
    t &= 0xff00ff;

    x = ((x >> 8) & 0xff) * a;
    x = (x + ((x >> 8) & 0xff) + 0x80);
    x &= 0xff00;
    x |= t | (a << 24);
    return x;
}

LH_Graph::LH_Graph( double defaultMin, double defaultMax )
{
    defaultMin_ = defaultMin;
    defaultMax_ = defaultMax;
    userDefinableLimits_ = false;
    graphMinY_ = graphMaxY_ = 0.0;
    values_.clear();
    cacheCount_.clear();
    cacheVal_.clear();
    len_ = 30;
    divisorY_ = 1;
    unitText_ = "";
    useLinkedValueAverage_ = false;
    return;
}

const char *LH_Graph::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;

    if (isDebug) qDebug() << "graph: init: begin";

    QStringList fgTypes = QStringList();
    fgTypes.append("Line Only");
    fgTypes.append("Area Fill");
    fgTypes.append("Image");
    setup_fg_type_ = new LH_Qt_QStringList(this, "Graph Style", fgTypes, LH_FLAG_AUTORENDER);
    setup_fg_type_->setHelp( "<p>The graph's style.</p><p style='color:red'>IMPORTANT NOTE: Using a foreground image for a large graph, especially one with multiple lines can have a significant impact on CPU usage.</p>");

    QStringList bgTypes = QStringList();
    bgTypes.append("None");
    bgTypes.append("Area Fill");
    bgTypes.append("Image");
    setup_bg_type_ = new LH_Qt_QStringList(this, "Graph Background", bgTypes, LH_FLAG_AUTORENDER);
    setup_bg_type_->setHelp( "<p>The graph's background style.</p>");
    setup_bg_type_->setValue(1);

    QStringList orientations = QStringList();
    orientations.append("Time Horizontal (Now = Right), Max Value = Top");
    orientations.append("Time Horizontal (Now = Right), Max Value = Bottom");
    orientations.append("Time Horizontal (Now = Left), Max Value = Top");
    orientations.append("Time Horizontal (Now = Left), Max Value = Bottom");
    orientations.append("Time Vertical (Now = Top), Max Value = Left");
    orientations.append("Time Vertical (Now = Top), Max Value = Right");
    orientations.append("Time Vertical (Now = Bottom), Max Value = Left");
    orientations.append("Time Vertical (Now = Bottom), Max Value = Right");
    setup_orientation_ = new LH_Qt_QStringList(this, "Graph Orientation", orientations, LH_FLAG_AUTORENDER);
    setup_orientation_->setHelp( "<p>The layout of the graph.</p>");

    setup_bgcolor_ = new LH_Qt_QColor(this,"Background color",Qt::transparent,LH_FLAG_AUTORENDER);
    setup_bgcolor_->setHelp( "<p>The color for the background.</p>");

    setup_bg_image_ = new LH_Qt_QFileInfo(this, "Background Image", QFileInfo(""), LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_bgcolor_->setHelp( "<p>The image used for the background.</p>");

    setup_max_samples_ = new LH_Qt_int(this,"Max Samples",len_,5,600,LH_FLAG_AUTORENDER);
    setup_max_samples_->setHelp( "<p>How many data points to store &amp; plot.</p>");

    setup_sample_rate_ = new LH_Qt_int(this,"Sample Rate",1,1,12,LH_FLAG_AUTORENDER);
    setup_sample_rate_->setHelp( "<p>How frequently to log data.</p>");

    setup_description_ = new LH_Qt_QString(this,"Description","...",LH_FLAG_READONLY|LH_FLAG_NOSAVE|LH_FLAG_NOSINK|LH_FLAG_NOSOURCE|LH_FLAG_HIDETITLE);
    setup_description_->setHelp( "<p>Combining the \"Max Samples\" and the \"Sample Rate\" this field displays that total timespan of the graph.</p>");

    setup_line_selection_ = new LH_Qt_QStringList(this,"Selected Line",QStringList());
    setup_line_selection_->setHelp( "<p>Select a line here and configure it below. Seperate settings are stored for each line.</p>");

    setup_line_pencolor_ = new LH_Qt_array_int_ui(this,"Line color",0,LH_FLAG_AUTORENDER, lh_type_integer_color);
    setup_line_pencolor_->setHelp( "<p>The colour used do draw the line.</p>");
    setup_line_fillcolor1_ = new LH_Qt_array_int_ui(this,"Area color (start)",0,LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN, lh_type_integer_color);
    setup_line_fillcolor1_->setHelp( "<p>The color used to fill the area between the line and the axis, at the furthest point from the axis (this color is only truely visible when the graph is full).</p>");
    setup_line_fillcolor2_ = new LH_Qt_array_int_ui(this,"Area color (end)",0,LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN, lh_type_integer_color);
    setup_line_fillcolor2_->setHelp( "<p>The color used to fill the area between the line and the axis, at the axis.</p>");

    setup_line_image_ = new LH_Qt_array_string_ui(this, "Fill Image", 0, LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN, lh_type_string);
    setup_line_image_->setHelp( "<p>This image is used to create the foreground, filling the area between the line and the axis.</p>");
    setup_line_image_opacity_ = new LH_Qt_array_int_ui(this,"Fill Image Opacity", 1, 0, 255, LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_line_image_opacity_->setHelp( "<p>This value affects the opacity of the fill image.</p>");

    setup_max_ = new LH_Qt_double(this, "Graph Ymax",defaultMax_,-99999999,99999999, LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);
    setup_max_->setHelp( "<p>The maximum value displayed on the graph.</p>"
                         "<p>This value can only be set when \"Ymax Can Grow\" is disabled (see below).</p>");

    setup_linked_values_ = new LH_Qt_array_double(this,"Linked Value",0,LH_FLAG_NOSAVE);
    setup_units_ = new LH_Qt_QStringList(this,"Units",QStringList(),LH_FLAG_AUTORENDER | LH_FLAG_HIDDEN);

    setup_max_grow_ = new LH_Qt_bool(this,"Ymax Can Grow", true, LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_HIDDEN);
    setup_max_grow_->setHelp( "<p>When enabled the value for \"Graph Ymax\" will increase automatically to ensure no value goes off the top edge.</p>");

    setup_auto_scale_y_max_ = new LH_Qt_bool(this,"Auto Scale Ymax", false, LH_FLAG_AUTORENDER);
    setup_auto_scale_y_max_->setHelp( "<p>When enabled, the plotted area's highest point will shift with the visible data to create a \"zooming\" effect. The less variation in the data the tighter the zoom. (Best used with \"Auto Scale Ymin\".)</p>");

    setup_min_ = new LH_Qt_double(this, "Graph Ymin",defaultMin_,-99999999,99999999, LH_FLAG_AUTORENDER | LH_FLAG_READONLY | LH_FLAG_HIDDEN);
    setup_min_->setHelp( "<p>The minimum value displayed on the graph.</p>");

    setup_auto_scale_y_min_ = new LH_Qt_bool(this,"Auto Scale Ymin", false, LH_FLAG_AUTORENDER);
    setup_auto_scale_y_min_->setHelp( "<p>When enabled, the plotted area's lowest point will shift with the visible data to create a \"zooming\" effect. The less variation in the data the tighter the zoom. (Best used with \"Auto Scale Ymax\".)</p>");

    setup_show_y_max_ = new LH_Qt_bool(this,"Show Ymax", false, LH_FLAG_AUTORENDER);
    setup_show_y_max_->setHelp( "<p>Displays the maximum visible point on the graph's <i>Y-axis</i>, i.e. the highest value that could be plotted without changing the scale.</p>"
                         "<p>Selecting \"Show Real Y-Limits\" will change this to displaying the highest value currently visible on the graph's <i>line(s)</i>, i.e. the highest value actually \"seen\".</p>");

    setup_show_y_min_ = new LH_Qt_bool(this,"Show Ymin", false, LH_FLAG_AUTORENDER);
    setup_show_y_min_->setHelp( "<p>Displays the minimum visible point on the graph's <i>Y-axis</i>, i.e. the lowest value that could be plotted without changing the scale.</p>"
                         "<p>Selecting \"Show Real Y-Limits\" will change this to displaying the lowest value currently visible on the graph's <i>line(s)</i>, i.e. the lowest value actually \"seen\".</p>");

    setup_hide_when_empty_ = new LH_Qt_bool(this,"Hide When Empty", false, LH_FLAG_AUTORENDER|LH_FLAG_HIDDEN);
    setup_hide_when_empty_->setHelp( "<p>When enabled, this will override the \"Show Ymax\" and \"Show Ymin\" values to hide the axis labels when the graph shows only a straight line at 0 (i.e. no valid data has been received).</p>");

    setup_show_real_limits_ = new LH_Qt_bool(this,"Show Real Y-Limits", false, LH_FLAG_AUTORENDER|LH_FLAG_HIDDEN);
    setup_show_real_limits_->setHelp( "<p>By default the labels shown on a graph correspond to the upper & lower values visible on the Y-axis, i.e. the highest & lowest values that could be plotted without changing the scale. This is most useful when a graph auto scales.</p>"
                                      "<p>However, some users may wish instead to show the highest & lowest points currently visible on the graph's line(s). This is particularly useful on graphs that are not set to auto scale.");

    setup_y_labels_right_ = new LH_Qt_bool(this,"Y Labels on Right", false, LH_FLAG_AUTORENDER|LH_FLAG_HIDDEN);
    setup_y_labels_right_->setHelp( "<p>Position the labels on the right side instead of the left (or bottom instead of top for graphs which swap the X & Y axis).</p>");

    setup_label_font_ = new LH_Qt_QFont(this, "Axis Label Font", QFont("Arial",8),LH_FLAG_AUTORENDER|LH_FLAG_HIDDEN);
    setup_label_font_->setHelp( "<p>The font used for Axis labels.</p>");

    setup_label_color_ = new LH_Qt_QColor(this,"Axis Label Color","white",LH_FLAG_AUTORENDER|LH_FLAG_HIDDEN);
    setup_label_color_->setHelp( "<p>The color used for Axis labels.</p>");

    setup_label_shadow_ = new LH_Qt_QColor(this,"Axis Label Glow",QColor(0,0,0,92),LH_FLAG_AUTORENDER|LH_FLAG_HIDDEN);
    setup_label_shadow_->setHelp( "<p>The color used for for the \"Glow/Shadow\" effect around Axis labels (designed to improve legibility).</p>"
                                  "<p>Set the transparency to 0 to remove the effect.</p>");

    addLine("Default");

    connect( setup_fg_type_, SIGNAL(changed()), this, SLOT(changeType()) );
    connect( setup_bg_type_, SIGNAL(changed()), this, SLOT(changeType()) );
    connect( setup_max_samples_, SIGNAL(changed()), this, SLOT(changeMaxSamples()) );
    connect( setup_sample_rate_, SIGNAL(changed()), this, SLOT(changeSampleRate()) );
    connect( setup_line_selection_, SIGNAL(changed()), this, SLOT(changeSelectedLine()) );
    connect( setup_bg_image_, SIGNAL(changed()), this, SLOT(reloadImages()) );
    connect( setup_show_y_max_, SIGNAL(changed()), this, SLOT(updateLabelSelection()) );
    connect( setup_show_y_min_, SIGNAL(changed()), this, SLOT(updateLabelSelection()) );
    connect( setup_show_real_limits_, SIGNAL(changed()), this, SLOT(updateLabelSelection()) );
    connect( setup_max_grow_, SIGNAL(changed()), this, SLOT(updateLimitControls()) );
    connect( setup_linked_values_, SIGNAL(changed()), this, SLOT(newLinkedValue()) );
    connect( setup_units_, SIGNAL(changed()), this, SLOT(changeUnits()));
    connect( setup_line_image_, SIGNAL(changed()), this, SLOT(reloadImages()));

    connect( this, SIGNAL(initialized()), this, SLOT(updateDescText()));

    if (isDebug) qDebug() << "graph: init: done";

    return 0;
}

double LH_Graph::max()
{
    if (setup_min_->value() < setup_max_->value())
        return setup_max_->value();
    else
        return setup_min_->value()+1;
}
double LH_Graph::max(double val)
{
    setup_max_->setValue(val);
    return max();
}

double LH_Graph::min()
{
    return setup_min_->value();
}
double LH_Graph::min(double val)
{
    setup_min_->setValue(val);
    return min();
}

bool LH_Graph::canGrow()
{
    return setup_max_grow_->value();
}
bool LH_Graph::canGrow(bool val)
{
    setup_max_grow_->setValue(val);
    updateLimitControls();
    return canGrow();
}

void LH_Graph::findDataBounds()
{
    if (isDebug) qDebug() << "graph: find bounds: begin";

    // apply fixes for cases where somehow graph limits have become NaN
    if(graphMinY_ != graphMinY_) graphMinY_ = min();
    if(graphMaxY_ != graphMaxY_) graphMaxY_ = max();

    // set the bounds such that min is suitably high (so it can be correctly
    // brought down) and max is suitably low (so it can be correctly raised)
    dataMinY_ = max();
    dataMaxY_ = min();

    // examine data points for each line and shift the data boundries accordingly
    for(int lineID=0;lineID<lineCount(); lineID++)
    {
        double valueMin = max();
        double valueMax = min();
        bool isConstant = true;
        double constantValue = 0;
        for(int i=0;i<values_[lineID].length() && i<len_;i++)
        {
            double y = values_[lineID].at(i);
            if(i==0)
                constantValue = y;
            else
                isConstant = isConstant && (constantValue == y);
            if(valueMin>y) valueMin = y;
            if(valueMax<y) valueMax = y;
        }
        bool doDraw = !(hasDeadValue_ && isConstant && (deadValue_ == constantValue));
        if(doDraw)
        {
            if(dataMinY_>valueMin) dataMinY_ = valueMin;
            if(dataMaxY_<valueMax) dataMaxY_ = valueMax;
        }
    }

    // if the set maximum value is allowed to move itself, apply any necessary movement
    if(canGrow() && dataMaxY_ > max()) max(dataMaxY_ * 1.1);

    // calculate the visible range drawn
    dataDeltaY_ = (setup_auto_scale_y_max_->value()? dataMaxY_ : max()) -
                  (setup_auto_scale_y_min_->value()? dataMinY_ : min());

    //set whether the graph is empty
    graph_empty_ = (dataMaxY_ == dataMinY_) && (dataMinY_ == 0);

    // if the range is 0 and is left as such the graph will experience weird behaviour
    // so to fix this a small alteration is made in this case.
    if(dataDeltaY_ == 0)
        dataDeltaY_ = 0.01 * (max()-min());

    //set the graph's max
    if (!setup_auto_scale_y_max_->value())
        //unzoomed
        graphMaxY_ = max();
    else if (dataMaxY_ > graphMaxY_ || dataMaxY_ + dataDeltaY_ * 0.667 < graphMaxY_)
    {
        //auto-zoom
        graphMaxY_ = dataMaxY_ + dataDeltaY_ * 0.333;
        if(canGrow() && graphMaxY_ > max())max(graphMaxY_ * 1.1);
        if(graphMaxY_ > max())
            graphMaxY_ = max();
    }

    //set the graph's min
    if (!setup_auto_scale_y_min_->value())
         //unzoomed
        graphMinY_ = min();
    else if (dataMinY_ < graphMinY_ || dataMinY_ - dataDeltaY_ * 0.667 > graphMinY_)
    {
        //auto-zoom
        graphMinY_ = dataMinY_ - dataDeltaY_ * 0.333;
        if(graphMinY_ < min())
            graphMinY_ = min();
    }

    // if a graph min = graph max has slipped through, fix it with a hammer here.
    if (graphMaxY_ == graphMinY_) graphMaxY_ += dataDeltaY_;

    if (isDebug) qDebug() << "graph: find bounds: done";
}

void LH_Graph::drawSingle( int lineID )
{
    if (lineID>=lineCount()) return;

    if (isDebug) qDebug() << "graph: draw line: begin " << lineID;
    QColor penColor = QColor();
    QColor fillColor1 = QColor();
    QColor fillColor2 = QColor();
    QString fgImgPath = "";
    int fgImgAlpha = 255;

    //get the colours required for this line & it's fill area
    loadColors(lineID, penColor, fillColor1, fillColor2, fgImgPath, fgImgAlpha);

    QPainter painter;

    QPointF points[len_+2];

    int w = image_->width();
    int h = image_->height();

    //empty the graph when drawing line 0
    if(lineID==0)
    {
        if (img_size_.width() != w || img_size_.height()!= h)
            reloadImages();

        delete image_;

        switch(setup_bg_type_->index())
        {
        case 2:
            if(setup_bg_image_->value().isFile())
            {
                image_ = new QImage(bgImg_);
                break;
            }
        case 0:
            image_ = new QImage(w,h,QImage::Format_ARGB32_Premultiplied);
            image_->fill( PREMUL( QColor(0,0,0,0).rgba() ) );
            break;
        case 1:
            image_ = new QImage(w,h,QImage::Format_ARGB32_Premultiplied);
            image_->fill( PREMUL( setup_bgcolor_->value().rgba() ) );
            break;
        }



        findDataBounds();
    }

    //assemble the array of points for the graph (based on values & orientation)
    bool isConstant = true;
    double constantValue = 0;
    int i;
    for(i=0;i<values_[lineID].length() && i<len_;i++)
    {
        double x = 0; double y=0;
        switch(setup_orientation_->index())
        {
        case 0:
            x = ((len_-1) - i) * w / (len_-1);
            y = h - (((values_[lineID].at(i) ) - graphMinY_) * h / (graphMaxY_ - graphMinY_));
            break;
        case 1:
            x = ((len_-1) - i) * w / (len_-1);
            y = (((values_[lineID].at(i) ) - graphMinY_) * h / (graphMaxY_ - graphMinY_));
            break;
        case 2:
            x = w - ((len_-1) - i) * w / (len_-1);
            y = h - (((values_[lineID].at(i) ) - graphMinY_) * h / (graphMaxY_ - graphMinY_));
            break;
        case 3:
            x = w - ((len_-1) - i) * w / (len_-1);
            y = (((values_[lineID].at(i) ) - graphMinY_) * h / (graphMaxY_ - graphMinY_));
            break;
        case 4:
            x = w - (((values_[lineID].at(i) ) - graphMinY_) * w / (graphMaxY_ - graphMinY_));
            y = h - ((len_-1) - i) * h / (len_-1);
            break;
        case 5:
            x = (((values_[lineID].at(i) ) - graphMinY_) * w / (graphMaxY_ - graphMinY_));
            y = h - ((len_-1) - i) * h / (len_-1);
            break;
        case 6:
            x = w - (((values_[lineID].at(i) ) - graphMinY_) * w / (graphMaxY_ - graphMinY_));
            y = ((len_-1) - i) * h / (len_-1);
            break;
        case 7:
            x = (((values_[lineID].at(i) ) - graphMinY_) * w / (graphMaxY_ - graphMinY_));
            y = ((len_-1) - i) * h / (len_-1);
            break;
        }
        if(i==0)
            constantValue = values_[lineID].at(i);
        else
            isConstant = isConstant && (constantValue == values_[lineID].at(i));
        points[i] = QPointF(x, y);
    }


    //apply point corrections & prep gradient
    QLinearGradient gradient;
    double x = points[i-1].x();
    double y = points[i-1].y();
    switch(setup_orientation_->index())
    {
    case 0:
        points[i++] =  QPointF(x, h+10);
        points[i++] =  QPointF(w, h+10);
        gradient.setStart( QPointF(0, 0) );
        gradient.setFinalStop( QPointF(0,h) );
        break;
    case 1:
        points[i++] =  QPointF(x, -10);
        points[i++] =  QPointF(w, -10);
        gradient.setStart( QPointF(0, h) );
        gradient.setFinalStop( QPointF(0,0) );
        break;
    case 2:
        points[i++] =  QPointF(x, h+10);
        points[i++] =  QPointF(0, h+10);
        gradient.setStart( QPointF(0, 0) );
        gradient.setFinalStop( QPointF(0,h) );
        break;
    case 3:
        points[i++] =  QPointF(x, -10);
        points[i++] =  QPointF(0, -10);
        gradient.setStart( QPointF(0, h) );
        gradient.setFinalStop( QPointF(0,0) );
        break;
    case 4:
        points[i++] =  QPointF(w+10, y);
        points[i++] =  QPointF(w+10, 0);
        gradient.setStart( QPointF( 0,0 ) );
        gradient.setFinalStop( QPointF(w, 0) );
        break;
    case 5:
        points[i++] =  QPointF(-10, y);
        points[i++] =  QPointF(-10, 0);
        gradient.setStart( QPointF(w, 0) );
        gradient.setFinalStop( QPointF(0,0) );
        break;
    case 6:
        points[i++] =  QPointF(w+10, y);
        points[i++] =  QPointF(w+10, h);
        gradient.setStart( QPointF( 0,0 ) );
        gradient.setFinalStop( QPointF(w, 0) );
        break;
    case 7:
        points[i++] =  QPointF(-10, y);
        points[i++] =  QPointF(-10, h);
        gradient.setStart( QPointF(w, 0) );
        gradient.setFinalStop( QPointF(0,0) );
        break;
    }
    gradient.setColorAt(0,fillColor2);
    gradient.setColorAt(1,fillColor1);

    if( painter.begin( image_ ) )
    {
        painter.setRenderHint( QPainter::Antialiasing, true );
        painter.setPen(penColor);

        //draw the line and fill the area if required
        bool doDraw = !(hasDeadValue_ && isConstant && (deadValue_ == constantValue));
        if (doDraw)
        {
            switch(setup_fg_type_->index())
            {
            case 0:
                painter.drawPolyline(points, values_[lineID].length());
                break;
            case 1:
                painter.setBrush(QBrush(gradient));
                painter.drawPolygon(points, values_[lineID].length()+2);
                break;
            case 2:
                {
                    QRectF graph_area = QRectF( 0,0, w, h );

                    //build mask
                    uchar *blank_data = (uchar[4]){0,0,0,0};
                    QImage maskImg = QImage(blank_data,1,1,QImage::Format_ARGB32).scaled(w,h);
                    QPainter maskPaint;
                    if( maskPaint.begin( &maskImg ) )
                    {
                        maskPaint.setRenderHint( QPainter::Antialiasing, true );
                        QColor maskCol = QColor(0,0,0,fgImgAlpha);
                        maskPaint.setPen(maskCol);
                        maskPaint.setBrush(QBrush(maskCol));
                        maskPaint.drawPolygon(points, values_[lineID].length()+2);
                        maskPaint.end();
                    }

                    //apply mask
                    if(!fgImgs_.contains(lineID) && QFileInfo(fgImgPath).isFile())
                         fgImgs_.insert(lineID,QImage(fgImgPath).scaled(w,h));
                    if(fgImgs_.contains(lineID))
                    {
                        QImage tempImg = fgImgs_.value(lineID);
                        QPainter tempPaint;
                        if( tempPaint.begin( &tempImg ) )
                        {
                            tempPaint.setCompositionMode(QPainter::CompositionMode_DestinationIn);
                            tempPaint.drawImage(graph_area, maskImg);
                            tempPaint.end();
                        }
                        painter.drawImage(graph_area, tempImg);
                    }
                    painter.drawPolyline(points, values_[lineID].length());
                }
                break;
            }
        }

        // when completing the last line, add any labels if required
        if(lineID == lineCount() - 1)
        {
            painter.setFont( setup_label_font_->value() );
            int flags = Qt::TextSingleLine|Qt::TextIncludeTrailingSpaces;

            QString maxLabel = getLabelText((setup_show_real_limits_->value()? dataMaxY_: graphMaxY_));
            QString minLabel = getLabelText((setup_show_real_limits_->value()? dataMinY_: graphMinY_));

            bool emptyHide = setup_hide_when_empty_->value() && graph_empty_;
            switch(setup_orientation_->index())
            {
            case 0:
            case 2:
                flags = flags | (setup_y_labels_right_->value() ? Qt::AlignRight : Qt::AlignLeft);
                if(setup_show_y_max_->value() && !emptyHide) addText(painter, image_->rect(), flags|Qt::AlignTop,    maxLabel);
                if(setup_show_y_min_->value() && !emptyHide) addText(painter, image_->rect(), flags|Qt::AlignBottom, minLabel);
                break;
            case 1:
            case 3:
                flags = flags | (setup_y_labels_right_->value() ? Qt::AlignRight : Qt::AlignLeft);
                if(setup_show_y_max_->value() && !emptyHide) addText(painter, image_->rect(), flags|Qt::AlignBottom, maxLabel);
                if(setup_show_y_min_->value() && !emptyHide) addText(painter, image_->rect(), flags|Qt::AlignTop,    minLabel);
                break;
            case 4:
            case 6:
                flags = flags | (setup_y_labels_right_->value() ? Qt::AlignTop : Qt::AlignBottom);
                if(setup_show_y_max_->value() && !emptyHide) addText(painter, image_->rect(), flags|Qt::AlignLeft,   maxLabel);
                if(setup_show_y_min_->value() && !emptyHide) addText(painter, image_->rect(), flags|Qt::AlignRight,  minLabel);
                break;
            case 5:
            case 7:
                flags = flags | (setup_y_labels_right_->value() ? Qt::AlignTop : Qt::AlignBottom);
                if(setup_show_y_max_->value()) addText(painter, image_->rect(), flags|Qt::AlignRight,  maxLabel);
                if(setup_show_y_min_->value()) addText(painter, image_->rect(), flags|Qt::AlignLeft,   minLabel);
                break;
            }
        }

        painter.end();
    }
    if (isDebug) qDebug() << "graph: draw line: done " << lineID;
}

QString LH_Graph::getLabelText(double val)
{
    int prec = 1 - int(log10(dataDeltaY_/ divisorY_));
    if (prec<0) prec = 0;

    QString str = QString::number(val / divisorY_,'f',prec);
    while( str.contains(".") && ( str.endsWith("0") || str.endsWith(".") ) ) str.remove(str.length()-1,1);
    return str + unitText_;
}

void LH_Graph::addText(QPainter& painter, QRect rect, int flags, QString text)
{
    if (isDebug) qDebug() << "graph: add label: begin ";
    painter.setPen( setup_label_shadow_->value() );
    addText(painter, rect, flags, text, 0 , 0);
    addText(painter, rect, flags, text, 1 , 0);
    addText(painter, rect, flags, text, 2 , 0);
    addText(painter, rect, flags, text, 0 , 1);
    addText(painter, rect, flags, text, 2 , 1);
    addText(painter, rect, flags, text, 0 , 2);
    addText(painter, rect, flags, text, 1 , 2);
    addText(painter, rect, flags, text, 2 , 2);

    painter.setPen( setup_label_color_->value() );
    addText(painter, rect, flags, text, 1 , 1);
    if (isDebug) qDebug() << "graph: add label: done ";
}

void LH_Graph::addText(QPainter& painter, QRect rect, int flags, QString text, int Xmod, int Ymod)
{
    QRect modRect = rect;
    modRect.setTopLeft(QPoint(modRect.left()+Xmod,modRect.top()+Ymod));
    modRect.setBottomRight(QPoint(modRect.right()+Xmod-2,modRect.bottom()+Ymod));
    painter.drawText( modRect, flags, text );
}

void LH_Graph::addLine(QString name)
{
    setup_line_selection_->list().append(name);
    setup_line_selection_->refreshList();
    cacheCount_.append(0);
    cacheVal_.append(0);
    values_.append( QList<double>() );

    setup_line_selection_->setFlag(LH_FLAG_HIDDEN, setup_line_selection_->list().count()==1);
    setup_line_selection_->setValue(0);
    syncLineDataArrays();
}

int LH_Graph::lineCount()
{
    return setup_line_selection_->list().count();
}

void LH_Graph::setLineCount(int count)
{
    if(count <= 0)
        return;
    if(count == setup_line_selection_->list().count())
        return;
    QStringList names;
    for(int i = 0; i<count;)
        names.append(QString("Line #%1").arg(++i));
    setLines(names);    
}

void LH_Graph::clearLines()
{
    setup_line_selection_->list().clear();
    setup_line_selection_->refreshList();
    cacheCount_.clear();
    cacheVal_.clear();
    values_.clear();
    setup_line_selection_->setFlag(LH_FLAG_HIDDEN, false);
    syncLineDataArrays();
}

bool LH_Graph::setMin( double r )
{
    if( min() == r ) return false;
    min(r);
    graphMinY_ = r;
    if(graphMaxY_==r) setMax(r+1, canGrow());
    return true;
}

bool LH_Graph::setMax( double r, bool b )
{
    if(!userDefinableLimits_) canGrow( b );
    if(graphMinY_==r) r++;
    if( max() == r ) return false;
    max(r);
    graphMaxY_ = r;
    return true;
}

void LH_Graph::setYUnit( QString str, double divisor )
{
    unitText_ = str;
    if (divisor!=0) divisorY_ = divisor;
}

int LH_Graph::notify(int n, void *p)
{
    Q_UNUSED(p);
    if(setup_linked_values_->link()==NULL)
        return 0;
    else
    {
        if(n&LH_NOTE_SECOND)
        {
            double totalTotal = 0;
            setLineCount(linkedValues.count());
            for(int n=0; n<linkedValues.count(); n++)
            {
                double total = 0;
                foreach(double d, linkedValues[n])
                    total += d;
                if(linkedValues[n].count()!=0)
                    total/=linkedValues[n].count();
                if(!useLinkedValueAverage_)
                    addValue(total, n);
                else
                    totalTotal += total;
                linkedValues[n].clear();
            }
            if(linkedValues.count()!=0)
                totalTotal/=linkedValues.count();
            if(useLinkedValueAverage_)
                addValue(totalTotal);
            requestRender();
        }
        return LH_NOTE_SECOND;
    }
}

QImage *LH_Graph::render_qimage( int w, int h )
{
    if( LH_QtInstance::initImage(w,h) == NULL ) return NULL;
    image_->fill( PREMUL( setup_bgcolor_->value().rgba() ) );
    drawAll();
    return image_;
}

void LH_Graph::addValue(double value, int lineID )
{
    if (lineID>=lineCount()) return;
    if (isDebug) qDebug() << "graph: add value: begin " << lineID;
    cacheCount_[lineID] ++;
    cacheVal_[lineID] += (double)value;
    if(cacheCount_[lineID] >= setup_sample_rate_->value())
    {
        if (values_[lineID].length()>=len_) values_[lineID].pop_back();
        values_[lineID].push_front(cacheVal_[lineID] / cacheCount_[lineID]);
        cacheCount_[lineID] = 0;
        cacheVal_[lineID] = 0;
    }
    if (isDebug) qDebug() << "graph: add value: end ";
}

void LH_Graph::loadColors(int lineID, QColor& penColor, QColor& fillColor1, QColor& fillColor2, QString& fgImgPath, int& fgImgAlpha)
{
    if (isDebug) qDebug() << "graph: load colours: begin " << lineID;

    penColor.setRgba(setup_line_pencolor_->at(lineID,0));
    fillColor1.setRgba(setup_line_fillcolor1_->at(lineID,0));
    fillColor2.setRgba(setup_line_fillcolor2_->at(lineID,0));
    fgImgPath = setup_line_image_->at(lineID,"");
    fgImgAlpha = setup_line_image_opacity_->at(lineID,255);

    if (isDebug) qDebug() << "graph: load colours: end ";
}

QString LH_Graph::buildColorConfig()
{

    QStringList config = QStringList();

    int lineID = setup_line_selection_->index();

    QColor penColor = QColor::fromRgba(setup_line_pencolor_->at(lineID, QColor(Qt::black).rgba()) );
    config.append(QString::number(penColor.red()));
    config.append(QString::number(penColor.green()));
    config.append(QString::number(penColor.blue()));
    config.append(QString::number(penColor.alpha()));

    QColor fillColor1 = QColor::fromRgba(setup_line_fillcolor1_->at(lineID, QColor(Qt::green).rgba()) );
    config.append(QString::number(fillColor1.red()));
    config.append(QString::number(fillColor1.green()));
    config.append(QString::number(fillColor1.blue()));
    config.append(QString::number(fillColor1.alpha()));

    QColor fillColor2 = QColor::fromRgba(setup_line_fillcolor2_->at(lineID, QColor(Qt::red).rgba()) );
    config.append(QString::number(fillColor2.red()));
    config.append(QString::number(fillColor2.green()));
    config.append(QString::number(fillColor2.blue()));
    config.append(QString::number(fillColor2.alpha()));

    config.append(setup_line_image_->at(lineID,""));
    config.append(QString::number(setup_line_image_opacity_->at(lineID,255)));

    return config.join(",");
}

void LH_Graph::changeMaxSamples()
{
    len_ = setup_max_samples_->value();

    for(int i =0; i<lineCount(); i++ )
    {
        while(len_<values_[i].length())
            values_[i].removeLast();
    }
    updateDescText();
}

void LH_Graph::changeSampleRate()
{
    updateDescText();
}

void LH_Graph::updateDescText()
{
    int sTotal = len_ * setup_sample_rate_->value();
    int s = sTotal % 60;
    int m = ((sTotal - s)/60) % 60;
    int h = ((sTotal - s)/60 - m) / 60;

    QString strH = QString("%1").arg(h);
    if (strH.length()==1) strH = "0" + strH;
    QString strM = QString("%1").arg(m);
    if (strM.length()==1) strM = "0" + strM;
    QString strS = QString("%1").arg(s);
    if (strS.length()==1) strS = "0" + strS;

    QString str1 = QString(" timespan (%1 samples @ ").arg(setup_max_samples_->value());
    QString str2 = QString("%1s intervals)").arg(setup_sample_rate_->value());

    setup_description_->setValue( strH + ":" +strM + ":" + strS + str1 + str2 );
}

void LH_Graph::changeType()
{
    setup_line_fillcolor1_->setFlag(LH_FLAG_HIDDEN, (setup_fg_type_->index()!=1));
    setup_line_fillcolor2_->setFlag(LH_FLAG_HIDDEN, (setup_fg_type_->index()!=1));
    setup_line_image_->setFlag(LH_FLAG_HIDDEN, (setup_fg_type_->index()!=2));
    setup_line_image_opacity_->setFlag(LH_FLAG_HIDDEN, (setup_fg_type_->index()!=2));

    setup_bgcolor_->setFlag(LH_FLAG_HIDDEN, (setup_bg_type_->index()!=1));
    setup_bg_image_->setFlag(LH_FLAG_HIDDEN, (setup_bg_type_->index()!=2));
}

void LH_Graph::changeSelectedLine()
{
    QColor penColor = QColor();
    QColor fillColor1 = QColor();
    QColor fillColor2 = QColor();
    QString fgImgPath = "";
    int fgImgAlpha = 255;

    if (setup_line_selection_->index() >= lineCount()) setup_line_selection_->setValue(lineCount()-1);
    if (setup_line_selection_->index() < 0) setup_line_selection_->setValue(0);

    loadColors(setup_line_selection_->index(), penColor, fillColor1, fillColor2, fgImgPath, fgImgAlpha);

    setup_line_pencolor_->setEditIndex(setup_line_selection_->index());
    setup_line_fillcolor1_->setEditIndex(setup_line_selection_->index());
    setup_line_fillcolor2_->setEditIndex(setup_line_selection_->index());
    setup_line_image_->setEditIndex(setup_line_selection_->index());
    setup_line_image_opacity_->setEditIndex(setup_line_selection_->index());
}

void LH_Graph::updateLabelSelection()
{
    setup_hide_when_empty_->setFlag(LH_FLAG_HIDDEN, !(setup_show_y_max_->value() | setup_show_y_min_->value()));
    setup_show_real_limits_->setFlag(LH_FLAG_HIDDEN, !(setup_show_y_max_->value() | setup_show_y_min_->value()));
    setup_y_labels_right_->setFlag(LH_FLAG_HIDDEN, !(setup_show_y_max_->value() | setup_show_y_min_->value()));
    setup_label_font_->setFlag(LH_FLAG_HIDDEN, !(setup_show_y_max_->value() | setup_show_y_min_->value()));
    setup_label_color_->setFlag(LH_FLAG_HIDDEN, !(setup_show_y_max_->value() | setup_show_y_min_->value()));
    setup_label_shadow_->setFlag(LH_FLAG_HIDDEN, !(setup_show_y_max_->value() | setup_show_y_min_->value()));
}

void LH_Graph::clear(double newMin, double newMax, bool newGrow)
{
    for(int lineID=0;lineID<lineCount(); lineID++)
    {
        values_[lineID].clear();
    }
    graphMinY_ = graphMaxY_ = 0.0;
    min(newMin);
    max(newMax);
    canGrow(newGrow);
}

bool LH_Graph::userDefinableLimits()
{
    return userDefinableLimits_;
}

bool LH_Graph::setUserDefinableLimits(bool v)
{
    if(userDefinableLimits_!=v)
    {
        userDefinableLimits_ = v;
        updateLimitControls();
        return true;
    }
    else
        return false;
}

void LH_Graph::updateLimitControls()
{
    setup_max_grow_->setFlag(LH_FLAG_HIDDEN, !userDefinableLimits_);
    setup_max_grow_->setFlag(LH_FLAG_READONLY, !userDefinableLimits_);
    setup_max_->setFlag(LH_FLAG_HIDDEN, !userDefinableLimits_);
    setup_max_->setFlag(LH_FLAG_READONLY, !userDefinableLimits_ || canGrow());
    setup_min_->setFlag(LH_FLAG_HIDDEN, !userDefinableLimits_);
    setup_min_->setFlag(LH_FLAG_READONLY, !userDefinableLimits_);
}

void LH_Graph::reloadImages()
{
    if(image_ == NULL)
        return;

    int w = image_->width();
    int h = image_->height();

    img_size_.setHeight(h);
    img_size_.setWidth(w);

    if(setup_bg_image_->value().isFile())
        bgImg_ = QImage(setup_bg_image_->value().absoluteFilePath()).scaled(w,h);

    for(int lineID=0;lineID<lineCount(); lineID++)
    {
        QColor penColor = QColor();
        QColor fillColor1 = QColor();
        QColor fillColor2 = QColor();
        QString fgImgPath = "";
        int fgImgAlpha = 255;

        loadColors(lineID, penColor, fillColor1, fillColor2, fgImgPath, fgImgAlpha);

        fgImgs_.remove(lineID);
        if(QFileInfo(fgImgPath).isFile())
            fgImgs_.insert(lineID, QImage(fgImgPath).scaled(w,h));
    }
}

void LH_Graph::newLinkedValue()
{
    while(linkedValues.count()<setup_linked_values_->size())
        linkedValues.append( QList<double>() );
    while(linkedValues.count()>setup_linked_values_->size())
        linkedValues.removeLast();

    for(int n=0; n<setup_linked_values_->size(); n++)
    {
        double val = setup_linked_values_->at(n);
        //val *= linkedValueMultiplier_;
        linkedValues[n].append(val * linkedValueMultiplier_);
        //qDebug() << linkedValueMultiplier_ << "*" << val << " = " << val * linkedValueMultiplier_;
    }
    callback(lh_cb_notify);
}

void LH_Graph::changeUnits()
{
    if(setup_units_->index()==-1)
        return;

    customUnit u = customUnits.at(setup_units_->index());
    setYUnit(u.text, u.divisor);
}

void LH_Graph::addCustomUnits(QString caption, QString text, double divisor)
{
    customUnits.append((customUnit){text, divisor});
    setup_units_->list().append(caption);
    setup_units_->refreshList();
    if(setup_units_->index()==-1)
        setup_units_->setIndex(0);
    setup_units_->setFlag(LH_FLAG_HIDDEN, false);
}

void LH_Graph::syncLineDataArrays()
{
    int _lineCount = lineCount();
    setup_line_pencolor_->resize(_lineCount, Qt::black);
    setup_line_fillcolor1_->resize(_lineCount, Qt::green);
    setup_line_fillcolor2_->resize(_lineCount, Qt::red);
    setup_line_image_->resize(_lineCount, "");
    setup_line_image_opacity_->resize(_lineCount, 255);

    qDebug() << "line.count: " <<_lineCount;
    /*
    QStringList configs = setup_line_configs_->value().split('~',QString::SkipEmptyParts);

    for(int lineID = 0; lineID < configs.length() && lineID < _lineCount; lineID++)
    {
        QString configString = configs.at(lineID);
        QStringList config = configString.split(',');
        qDebug() << "line["<< lineID <<"].config: " <<configString;

        QColor penColor;
        penColor.setRed(QString(config.at(0)).toInt());
        penColor.setGreen(QString(config.at(1)).toInt());
        penColor.setBlue(QString(config.at(2)).toInt());
        penColor.setAlpha(QString(config.at(3)).toInt());
        setup_line_pencolor_->setAt(lineID, penColor.rgba());
        qDebug() << "line["<< lineID <<"].color: " <<penColor.rgba();

        QColor fillColor1;
        fillColor1.setRed(QString(config.at(4)).toInt());
        fillColor1.setGreen(QString(config.at(5)).toInt());
        fillColor1.setBlue(QString(config.at(6)).toInt());
        fillColor1.setAlpha(QString(config.at(7)).toInt());
        setup_line_fillcolor1_->setAt(lineID, fillColor1.rgba());
        qDebug() << "line["<< lineID <<"].fill[1]: " <<fillColor1.rgba();

        QColor fillColor2;
        fillColor2.setRed(QString(config.at(8)).toInt());
        fillColor2.setGreen(QString(config.at(9)).toInt());
        fillColor2.setBlue(QString(config.at(10)).toInt());
        fillColor2.setAlpha(QString(config.at(11)).toInt());
        setup_line_fillcolor2_->setAt(lineID, fillColor2.rgba());
        qDebug() << "line["<< lineID <<"].fill[2]: " <<fillColor2.rgba();

        setup_line_image_->setAt( lineID, config.at(12));
        setup_line_image_opacity_->setAt(lineID, config.at(13).toInt());
    }*/
}
