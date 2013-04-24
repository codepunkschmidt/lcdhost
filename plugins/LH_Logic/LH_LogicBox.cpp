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
    hide();
}

const char *LH_LogicBox::userInit()
{
    if( const char *err = LH_QtCFInstance::userInit() ) return err;

    setup_input1_ = new LH_Qt_QString(this,"Input 1","", LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE_DATA);
    setup_input1_->setHelp("<p>This field is one of the inputs to the Logic Box.</p> <p>Link this item to another item's value, then you can perform a logic operation using the rules to affect the \"Output\" field.</p>");
    setup_input2_ = new LH_Qt_QString(this,"Input 2","", LH_FLAG_NOSOURCE | LH_FLAG_NOSAVE_DATA);
    setup_input2_->setHelp("<p>This field is one of the inputs to the Logic Box.</p> <p>Link this item to another item's value, then you can perform a logic operation using the rules to affect the \"Output\" field.</p>");

    setup_output_ = new LH_Qt_QString(this,"Output","", LH_FLAG_NOSINK | LH_FLAG_NOSAVE_DATA);
    setup_output_->setHelp("<p>This field is the output the Logic Box.</p> <p>You should \"derive\" this value using the inputs and your custom rules and then set it as a data source to be used elsewhere in your layout as you require.</p>");

    add_cf_source(setup_input1_);
    add_cf_source(setup_input2_);
    add_cf_target(setup_output_);

    setup_cf_enabled_->setValue(true);
    setup_cf_enabled_->setVisible(false);

    connect(this, SIGNAL(initialized()), this, SLOT(refreshOutput()));

    return 0;
}

void LH_LogicBox::refreshOutput()
{
    setup_output_->setValue("");
}

QImage *LH_LogicBox::render_qimage(int w, int h)
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
