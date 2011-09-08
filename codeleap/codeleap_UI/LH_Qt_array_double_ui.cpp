#include "LH_Qt_array_double_ui.h"

void LH_Qt_array_double_ui::init(lh_meta_type ui_type, int uiFlags, bool _fixedRange, double min, double max )
{
    fixedRange_ = _fixedRange;
    ui_ = NULL;
    QString uiIdent = QString("%1__ui__").arg(ident());
    if(ui_type == lh_type_double && !fixedRange_)
        ui_ = new LH_Qt_double(parent(), uiIdent.toUtf8(), 0, uiFlags );
    if(ui_type == lh_type_double && fixedRange_)
        ui_ = new LH_Qt_double(parent(), uiIdent.toUtf8(), 0, min, max, uiFlags );

    Q_ASSERT(ui_ != NULL);

    ui_->setTitle(ident());
    uiIndex_ = 0;

    connect(this, SIGNAL(changed()), this, SLOT(arrayValuesChanged()));
    connect(this, SIGNAL(set()), this, SLOT(arrayValuesChanged()));
    connect(ui_, SIGNAL(changed()), this, SLOT(uiValueChanged()));
}

void LH_Qt_array_double_ui::setEditIndex(int index) {
    uiIndex_ = index;
    arrayValuesChanged();
}

void LH_Qt_array_double_ui::setFlag(int f, bool state)
{
    if(f != LH_FLAG_HIDDEN)
        LH_Qt_array_double::setFlag(f, state);
    ui_->setFlag(f, state);
}

void LH_Qt_array_double_ui::setHelp(const QString& s)
{
    LH_Qt_array_double::setHelp(s);
    ui_->setHelp(s);
}

void LH_Qt_array_double_ui::setTitle(const char *s)
{
    LH_Qt_array_double::setTitle(s);
    ui_->setTitle(s);
}

void LH_Qt_array_double_ui::setTitle( const QString &s )
{
    LH_Qt_array_double::setTitle(s);
    ui_->setTitle(s);
}

void LH_Qt_array_double_ui::arrayValuesChanged()
{
    if (uiIndex_>=0 && uiIndex_ < size())
    {
        ui_->blockSignals(true);
        if (ui_->type() == lh_type_double && !fixedRange_)
            reinterpret_cast<LH_Qt_double*>(ui_)->setMinMax(min(),max());
        if(ui_->type() == lh_type_double)
            reinterpret_cast<LH_Qt_double*>(ui_)->setValue(at(uiIndex_));
        ui_->blockSignals(false);
    }
}

void LH_Qt_array_double_ui::uiValueChanged()
{
    if (uiIndex_>=0 && uiIndex_ < size())
    {
        if(ui_->type() == lh_type_double)
            setAt(uiIndex_, reinterpret_cast<LH_Qt_double*>(ui_)->value());
    }
}
