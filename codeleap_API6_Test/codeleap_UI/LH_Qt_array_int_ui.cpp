#include "LH_Qt_array_int_ui.h"

void LH_Qt_array_int_ui::init(lh_setup_type ui_type, int uiFlags, qint64 min, qint64 max )
{
    QString uiIdent = QString("%1__ui__").arg(ident());
    if( ui_type == lh_type_integer )
    {
        if( uiFlags & LH_FLAG_MINMAX )
            ui_ = new LH_Qt_int(parent(), uiIdent.toUtf8(), 0, min, max, uiFlags );
        else
            ui_ = new LH_Qt_int(parent(), uiIdent.toUtf8(), 0, uiFlags );
    }
    else if( ui_type == lh_type_integer_color )
        ui_ = new LH_Qt_QColor(parent(), uiIdent.toUtf8(), QColor::fromRgba(0), uiFlags );

    Q_ASSERT( ui_ );

    connect( this, SIGNAL(titleChanged(const char*)), ui_, SLOT(setTitle(const char*)) );
    connect( this, SIGNAL(helpSet(const char*)), ui_, SLOT(setHelp(const char*)) );
    connect( this, SIGNAL(valueChanged()), this, SLOT(arrayValuesChanged()) );
    connect( this, SIGNAL(valueSet()), this, SLOT(arrayValuesChanged()) );
    connect( ui_, SIGNAL(valueChanged()), this, SLOT(uiValueChanged()) );
    ui_->setTitle( title() );
}

void LH_Qt_array_int_ui::setEditIndex(int index)
{
    uiIndex_ = index;
    arrayValuesChanged();
}

void LH_Qt_array_int_ui::setFlag(int f, bool state)
{
    if(f != LH_FLAG_HIDDEN)
        LH_Qt_array_int::setFlag(f, state);
    ui_->setFlag(f, state);
}

void LH_Qt_array_int_ui::setMin( double d)
{
    LH_Qt_array_int::setMin(d);
    ui_->setMinimum(d);
}
void LH_Qt_array_int_ui::setMax( double d)
{
    LH_Qt_array_int::setMax(d);
    ui_->setMaximum(d);
}
void LH_Qt_array_int_ui::setMinMax( double a, double b)
{
    LH_Qt_array_int::setMinMax(a,b);
    ui_->setMinMax(a,b);
}

void LH_Qt_array_int_ui::setMin( qint64 n)
{
    LH_Qt_array_int::setMin(n);
    ui_->setMinimum(n);
}

void LH_Qt_array_int_ui::setMax( qint64 n)
{
    LH_Qt_array_int::setMax(n);
    ui_->setMaximum(n);
}

void LH_Qt_array_int_ui::setMinMax( qint64 a, qint64 b)
{
    LH_Qt_array_int::setMinMax(a,b);
    ui_->setMinMax(a,b);
}

void LH_Qt_array_int_ui::arrayValuesChanged()
{
    if( uiIndex_>=0 && uiIndex_ < size() )
    {
        ui_->blockSignals(true);
        if( flags()&LH_FLAG_MIN ) ui_->setMinimum( min() );
        else ui_->setFlag( LH_FLAG_MIN, false );
        if( flags()&LH_FLAG_MAX ) ui_->setMaximum( max() );
        else ui_->setFlag( LH_FLAG_MAX, false );
        ui_->setValue( at(uiIndex_) );
        ui_->blockSignals(false);
    }
}

void LH_Qt_array_int_ui::uiValueChanged()
{
    if( uiIndex_>=0 && uiIndex_ < size() )
    {
        setAt( uiIndex_, ui_->value() );
    }
}
