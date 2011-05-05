#include "LH_DataViewerBar.h"
// #include <windows.h>

LH_PLUGIN_CLASS(LH_DataViewerBar)

lh_class *LH_DataViewerBar::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Dynamic/DataViewer",
        "DataViewerBar",
        "Data Bar",
        48,48,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_DataViewerBar::LH_DataViewerBar(const char* name) : LH_Bar(name), data_(this)
{
    setup_lookup_code_ = new LH_Qt_QString( this, "Data Template", "");
    setup_lookup_code_->setOrder(-3);

    setup_item_name_ = new LH_Qt_QString( this, "Item" , "", LH_FLAG_READONLY|LH_FLAG_NOSAVE);
    setup_item_name_->setOrder(-3);

    setup_value_min_ = new LH_Qt_QString(this,"Min Value/Index","0");
    setup_value_min_->setOrder(-3);

    setup_item_name_min_ = new LH_Qt_QString( this, "Min" , "", LH_FLAG_READONLY|LH_FLAG_NOSAVE);
    setup_item_name_min_->setOrder(-3);

    setup_value_max_ = new LH_Qt_QString(this,"Max Value/Index","100");
    setup_value_max_->setOrder(-3);

    setup_item_name_max_ = new LH_Qt_QString( this, "Max" , "", LH_FLAG_READONLY|LH_FLAG_NOSAVE);
    setup_item_name_max_->setOrder(-3);

    connect( setup_lookup_code_, SIGNAL(changed()), this, SLOT(updateBar()) );
    connect( setup_value_min_, SIGNAL(changed()), this, SLOT(updateBar()) );
    connect( setup_value_max_, SIGNAL(changed()), this, SLOT(updateBar()) );

    setMin(0.0);
    setMax(100.0);
}

int LH_DataViewerBar::polling()
{
    updateBar();
    return polling_rate;
};

QImage *LH_DataViewerBar::render_qimage( int w, int h )
{
    if( LH_Bar::render_qimage(w,h) == NULL ) return NULL;
    drawSingle(barValue_);
    return image_;
}

void LH_DataViewerBar::updateBar()
{
    bool rerender = false;
    QString currTemplate = QString("%1/%2/%3").arg(setup_lookup_code_->value()).arg(setup_value_min_->value()).arg(setup_value_max_->value());
    if (data_.open() && data_.valid(currTemplate))
    {
        qreal newMin = toReal( data_.populateLookupCode(setup_value_min_->value()) , 0);
        setup_item_name_min_->setValue( data_.populateLookupCode(setup_value_min_->value(), true) );
        rerender = rerender | (newMin!=barMin_);
        barMin_ = newMin;
        setMin( barMin_ );

        qreal newMax = toReal( data_.populateLookupCode(setup_value_max_->value()) , 1);
        setup_item_name_max_->setValue( data_.populateLookupCode(setup_value_max_->value(), true) );
        rerender = rerender | (newMin!=barMax_);
        barMax_ = newMax;
        setMax( barMax_ );

        qreal newValue = toReal( data_.populateLookupCode(setup_lookup_code_->value()), 0 );
        setup_item_name_->setValue( data_.populateLookupCode(setup_lookup_code_->value(), true) );
        rerender = rerender | (newValue!=barValue_);
        barValue_= newValue;
    }
    if (rerender) callback(lh_cb_render,NULL);
}

qreal LH_DataViewerBar::toReal(QString str, qreal defaultVal)
{
    bool ok;
    qreal result = str.toDouble(&ok);
    if (ok)
        return result;
    else {
        result = str.toFloat(&ok);
        if (ok)
            return result;
        else
            return defaultVal;
    }
}


