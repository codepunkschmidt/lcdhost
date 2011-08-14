#include "LH_Qt_array_int_ui.h"

void LH_Qt_array_int_ui::init(lh_setup_type ui_type, int uiFlags, bool _fixedRange, qint64 min, qint64 max )
{
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


void LH_Qt_array_int_ui::setMin( double d)
{
    LH_Qt_array_int::setMin(d);
    ui_->setMin(d);
}
void LH_Qt_array_int_ui::setMax( double d)
{
    LH_Qt_array_int::setMax(d);
    ui_->setMax(d);
}
void LH_Qt_array_int_ui::setMinMax( double a, double b)
{
    LH_Qt_array_int::setMinMax(a,b);
    ui_->setMinMax(a,b);
}

void LH_Qt_array_int_ui::setMin( qint64 n)
{
    LH_Qt_array_int::setMin(n);
    ui_->setMin(n);
}
void LH_Qt_array_int_ui::setMax( qint64 n)
{
    LH_Qt_array_int::setMax(n);
    ui_->setMax(n);
}
void LH_Qt_array_int_ui::setMinMax( qint64 a, qint64 b)
{
    LH_Qt_array_int::setMinMax(a,b);
    ui_->setMinMax(a,b);
}

void LH_Qt_array_int_ui::arrayValuesChanged()
{
    if (uiIndex_>=0 && uiIndex_ < size())
    {
        ui_->blockSignals(true);
        if (ui_->type() == lh_type_integer && !fixedRange_)
            reinterpret_cast<LH_Qt_int*>(ui_)->setMinMax(min(),max());
        if(ui_->type() == lh_type_integer)
            reinterpret_cast<LH_Qt_int*>(ui_)->setValue(at(uiIndex_));
        if(ui_->type() == lh_type_integer_color)
            reinterpret_cast<LH_Qt_QColor*>(ui_)->setValue(QColor::fromRgba(at(uiIndex_)));
        ui_->blockSignals(false);
    }
}

void LH_Qt_array_int_ui::uiValueChanged()
{
    if (uiIndex_>=0 && uiIndex_ < size())
    {
        if(ui_->type() == lh_type_integer)
            setAt(uiIndex_, reinterpret_cast<LH_Qt_int*>(ui_)->value());
        if(ui_->type() == lh_type_integer_color)
            setAt(uiIndex_, reinterpret_cast<LH_Qt_QColor*>(ui_)->value().rgba());
    }
}
