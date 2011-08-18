#include "LH_Qt_array_string_ui.h"

void LH_Qt_array_string_ui::init(lh_setup_type ui_type, int uiFlags, const QStringList& listItems )
{
    if(ui_type == lh_type_string)
        ui_ = new LH_Qt_QString(parent(), QString("%1__ui__").arg(ident()).toUtf8(),"", uiFlags );
    if(ui_type == lh_type_string_filename)
        ui_ = new LH_Qt_QFileInfo(parent(), QString("%1__ui__").arg(ident()).toUtf8(),QFileInfo(), uiFlags );
    if(ui_type == lh_type_string_list)
        ui_ = new LH_Qt_QStringList(parent(), QString("%1__ui__").arg(ident()).toUtf8(), listItems, uiFlags );

    Q_ASSERT( ui_ );

    connect( this, SIGNAL(titleChanged(const char*)), ui_, SLOT(setTitle(const char*)) );
    connect( this, SIGNAL(helpChanged(const char*)), ui_, SLOT(setHelp(const char*)) );
    connect( this, SIGNAL(changed()), this, SLOT(arrayValuesChanged()) );
    connect( this, SIGNAL(set()), this, SLOT(arrayValuesChanged()) );
    connect( ui_, SIGNAL(changed()), this, SLOT(uiValueChanged()) );
    ui_->setTitle( title() );
}

int LH_Qt_array_string_ui::indexAt(int i)
{
    if(ui_->type()!=lh_type_string_list)
        return -1;
    return (reinterpret_cast<LH_Qt_QStringList*>(ui_)->list().indexOf( at(i) ));
}

int LH_Qt_array_string_ui::currentIndex()
{
    if(ui_->type()!=lh_type_string_list)
        return -1;
    return (reinterpret_cast<LH_Qt_QStringList*>(ui_)->index());
}

void LH_Qt_array_string_ui::setCurrentIndex(int i)
{
    if(ui_->type()==lh_type_string_list)
        (reinterpret_cast<LH_Qt_QStringList*>(ui_)->setIndex( i ));
}

void LH_Qt_array_string_ui::setEditIndex(int index)
{
    uiIndex_ = index;
    arrayValuesChanged();
}

void LH_Qt_array_string_ui::setFlag(int f, bool state)
{
    if(f != LH_FLAG_HIDDEN)
        LH_Qt_array_string::setFlag(f, state);
    ui_->setFlag(f, state);
}

void LH_Qt_array_string_ui::arrayValuesChanged()
{
    if( uiIndex_>=0 && uiIndex_ < size() )
    {
        ui_->blockSignals(true);
        if(ui_->type() == lh_type_string_filename)
            reinterpret_cast<LH_Qt_QFileInfo*>(ui_)->setValue( QFileInfo( QDir( dir_layout() ), at(uiIndex_) ) );
        else
            ui_->setValue( at(uiIndex_) );
        ui_->blockSignals(false);
    }
}

void LH_Qt_array_string_ui::uiValueChanged()
{
    if( uiIndex_>=0 && uiIndex_<size() )
    {
        setAt( uiIndex_, ui_->value() );
    }
}
