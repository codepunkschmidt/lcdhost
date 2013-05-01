#include "LH_Ticker.h"
#include <limits>

LH_PLUGIN_CLASS(LH_Ticker)

lh_class *LH_Ticker::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Miscellaneous/Logic",
        "Ticker",
        "Ticker",
        -1, -1,
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_Ticker::LH_Ticker()
{
    hide();
}

const char *LH_Ticker::userInit()
{
    if( const char *err = LH_QtCFInstance::userInit() ) return err;

    setup_ticks_ = new LH_Qt_int(this,"Ticks",0, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), LH_FLAG_NOSAVE_DATA);
    setup_rate_ = new LH_Qt_int(this,"Rate (ms)",1000, 50, 60000);
    setup_increment_ = new LH_Qt_int(this,"Increment",1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    connect( setup_rate_, SIGNAL(changed()), this, SLOT(changePolling()) );
    connect( setup_rate_, SIGNAL(set()), this, SLOT(changePolling()) );

    add_cf_source(setup_ticks_);
    add_cf_target(setup_ticks_);
    add_cf_source(setup_increment_);
    add_cf_target(setup_increment_);
    add_cf_source(setup_rate_);
    add_cf_target(setup_rate_);

    hide();
    return 0;
}

QImage *LH_Ticker::render_qimage(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
    if(QImage *img = initImage(1, 1))
    {
        img->fill(Qt::transparent);
        return img;
    }
    return 0;
}

int LH_Ticker::polling()
{
    int value = setup_ticks_->value();
    int inc = setup_increment_->value();

    if(inc>0)
    {
        int max = std::numeric_limits<int>::max();
        if(value <= max - inc)
            value += inc;
        else
            value = max;
    }
    if(inc<0)
    {
        int min = std::numeric_limits<int>::min();
        if(value >= min - inc)
            value += inc;
        else
            value = min;
    }
    setup_ticks_->setValue(value);

    return setup_rate_->value();
}

void LH_Ticker::changePolling()
{
    callback(lh_cb_polling,NULL);
}
