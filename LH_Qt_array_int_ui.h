#ifndef LH_QT_ARRAY_INT_UI_H
#define LH_QT_ARRAY_INT_UI_H

#include "LH_Qt_array_int.h"
#include "LH_Qt_int.h"
#include "LH_Qt_QColor.h"
#include <QDebug>

class LH_Qt_array_int_ui : public LH_Qt_array_int
{
    Q_OBJECT

    int uiIndex_;
    LH_QtSetupItem *ui_;
    bool updatingUI_;
    lh_setup_type ui_type_;

    bool fixedRange_;

    void init(lh_setup_type ui_type, int uiFlags, bool _fixedRange = false, qint64 min = 0, qint64 max = 0 )
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

public:
    LH_Qt_array_int_ui( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0, lh_setup_type ui_type = lh_type_integer)
        : LH_Qt_array_int( parent, ident, size, flags | LH_FLAG_HIDDEN )
    {
        init(ui_type, LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | flags);
    }

    LH_Qt_array_int_ui( LH_QtObject *parent, const char *ident, int size = 0, qint64 min = 0, qint64 max = 0, int flags = 0, lh_setup_type ui_type = lh_type_integer )
        : LH_Qt_array_int( parent, ident, size, flags | LH_FLAG_HIDDEN )
    {
        init(ui_type, LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | flags, true, min, max);
    }

    void setEditIndex(int index) {
        uiIndex_ = index;
        arrayValuesChanged();
    }

    void setFlag(int f, bool state)
    {
        if(f != LH_FLAG_HIDDEN)
            LH_Qt_array_int::setFlag(f, state);
        ui_->setFlag(f, state);
    }

    virtual void setHelp(const QString& s)
    {
        LH_Qt_array_int::setHelp(s);
        ui_->setHelp(s);
    }

    void setTitle(const char *s = 0)
    {
        LH_Qt_array_int::setTitle(s);
        ui_->setTitle(s);
    }

    void setTitle( const QString &s )
    {
        LH_Qt_array_int::setTitle(s);
        ui_->setTitle(s);
    }

public slots:
    void arrayValuesChanged() {
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

    void uiValueChanged(){
        if (updatingUI_) return;
        if (uiIndex_>=0 && uiIndex_ < this->size())
        {
            if(ui_type_ == lh_type_integer)
                this->setAt(uiIndex_, ((LH_Qt_int*)ui_)->value());
            if(ui_type_ == lh_type_integer_color)
                this->setAt(uiIndex_, ((LH_Qt_QColor*)ui_)->value().rgba());

        }
    }

};

#endif // LH_QT_ARRAY_INT_UI_H
