#include "LH_Qt_array_int_ui.h"

void LH_Qt_array_int_ui::init(lh_setup_type ui_type, int uiFlags, bool _fixedRange, qint64 min, qint64 max )
{
    ui_type_ = ui_type;
    fixedRange_ = _fixedRange;
    ui_ = NULL;
    QString uiIdent = QString("%1__ui__").arg(ident());
    if(ui_type == lh_type_integer && !fixedRange_)
        ui_ = new LH_Qt_int(parent(), uiIdent.toUtf8(), 0, uiFlags );
    if(ui_type == lh_type_integer && fixedRange_)
        ui_ = new LH_Qt_int(parent(), uiIdent.toUtf8(), 0, min, max, uiFlags );
    if(ui_type == lh_type_integer_color)
        ui_ = new LH_Qt_QColor(parent(), uiIdent.toUtf8(), QColor::fromRgba(0), uiFlags );

    Q_ASSERT(ui_ != NULL);

    ui_->setTitle(ident());
    uiIndex_ = 0;
    updatingUI_ = false;

    connect(this, SIGNAL(changed()), this, SLOT(arrayValuesChanged()));
    connect(this, SIGNAL(set()), this, SLOT(arrayValuesChanged()));
    connect(ui_, SIGNAL(changed()), this, SLOT(uiValueChanged()));
}

void LH_Qt_array_int_ui::setEditIndex(int index) {
    uiIndex_ = index;
    arrayValuesChanged();
}

void LH_Qt_array_int_ui::setFlag(int f, bool state)
{
    if(f != LH_FLAG_HIDDEN)
        LH_Qt_array_int::setFlag(f, state);
    ui_->setFlag(f, state);
}

void LH_Qt_array_int_ui::setHelp(const QString& s)
{
    LH_Qt_array_int::setHelp(s);
    ui_->setHelp(s);
}

void LH_Qt_array_int_ui::setTitle(const char *s)
{
    LH_Qt_array_int::setTitle(s);
    ui_->setTitle(s);
}

void LH_Qt_array_int_ui::setTitle( const QString &s )
{
    LH_Qt_array_int::setTitle(s);
    ui_->setTitle(s);
}

void LH_Qt_array_int_ui::arrayValuesChanged()
{
    if (uiIndex_>=0 && uiIndex_ < this->size())
    {
        updatingUI_ = true;
        if (ui_type_ == lh_type_integer && !fixedRange_)
            ((LH_Qt_int*)ui_)->setMinMax(this->min(),this->max());
        if(ui_type_ == lh_type_integer)
            ;//((LH_Qt_int*)ui_)->setValue((this->at(uiIndex_))); // causes assert failure!
        if(ui_type_ == lh_type_integer_color)
            ((LH_Qt_QColor*)ui_)->setValue(QColor::fromRgba(this->at(uiIndex_)));
        updatingUI_ = false;
    }
}

void LH_Qt_array_int_ui::uiValueChanged()
{
    if (updatingUI_) return;
    if (uiIndex_>=0 && uiIndex_ < this->size())
    {
        if(ui_type_ == lh_type_integer)
            this->setAt(uiIndex_, ((LH_Qt_int*)ui_)->value());
        if(ui_type_ == lh_type_integer_color)
            this->setAt(uiIndex_, ((LH_Qt_QColor*)ui_)->value().rgba());
    }
}
