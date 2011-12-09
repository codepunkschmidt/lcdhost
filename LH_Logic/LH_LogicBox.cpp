#include "LH_LogicBox.h"

LH_PLUGIN_CLASS(LH_LogicBox)

lh_class *LH_LogicBox::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Miscellaneous/Logic",
        "LogicBox",
        "Logic Box",
        -1, -1,
        lh_object_calltable_NULL,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_LogicBox::LH_LogicBox()
{

}

const char *LH_LogicBox::userInit()
{
    if( const char *err = LH_QtCFInstance::userInit() ) return err;
    hide();

    setup_input1_ = new LH_Qt_QString(this,"Input 1","", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE);
    setup_input2_ = new LH_Qt_QString(this,"Input 2","", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE);

    setup_output_ = new LH_Qt_QString(this,"Output","", LH_FLAG_NOSAVE | LH_FLAG_NOSINK);

    add_cf_source(setup_input1_);
    add_cf_source(setup_input2_);
    add_cf_target(setup_output_);

    setup_cf_enabled_->setValue(true);
    setup_cf_enabled_->setVisible(false);

    return 0;
}
