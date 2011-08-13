#ifndef LH_QT_ARRAY_DOUBLE_UI_H
#define LH_QT_ARRAY_DOUBLE_UI_H

#include "LH_Qt_array_double.h"
#include "LH_Qt_double.h"

class LH_Qt_array_double_ui : public LH_Qt_array_double
{
    Q_OBJECT

    int uiIndex_;
    LH_Qt_double *ui_;
    bool updatingUI_;

public:

    LH_Qt_array_double_ui( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0 )
        : LH_Qt_array_double( parent, ident, size, flags | LH_FLAG_HIDDEN)
    {
        ui_ = new LH_Qt_double(parent, QString("%1__ui__").arg(ident).toUtf8(),0, LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | flags );
        ui_->setTitle(ident);
        uiIndex_ = 0;
        updatingUI_ = false;

        connect(this, SIGNAL(changed()), this, SLOT(arrayValuesChanged()));
        connect(this, SIGNAL(set()), this, SLOT(arrayValuesChanged()));
        connect(ui_, SIGNAL(changed()), this, SLOT(uiValueChanged()));
    }

    void setEditIndex(int index) { uiIndex_ = index; arrayValuesChanged(); }

    void setFlag(int f, bool state)
    {
        if(f != LH_FLAG_HIDDEN)
            LH_Qt_array_double::setFlag(f, state);
        ui_->setFlag(f, state);
    }

    void setTitle(const char *s = 0)
    {
        LH_Qt_array_double::setTitle(s);
        ui_->setTitle(s);
    }

    void setTitle( const QString &s )
    {
        LH_Qt_array_double::setTitle(s);
        ui_->setTitle(s);
    }

public slots:
    void arrayValuesChanged() {
        if (uiIndex_>=0 && uiIndex_ < this->size())
        {
            updatingUI_ = true;
            ui_->setMinMax(this->min(),this->max());
            ui_->setValue(this->at(uiIndex_));
            updatingUI_ = false;
        }
    }

    void uiValueChanged(){
        if (updatingUI_) return;
        if (uiIndex_>=0 && uiIndex_ < this->size())
           this->setAt(uiIndex_, ui_->value());
    }

};

#endif // LH_QT_ARRAY_DOUBLE_UI_H
