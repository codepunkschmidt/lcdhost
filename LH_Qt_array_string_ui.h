#ifndef LH_QT_ARRAY_STRING_UI_H
#define LH_QT_ARRAY_STRING_UI_H

#include "LH_Qt_array_string.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QFileInfo.h"
#include <QDebug>

class LH_Qt_array_string_ui : public LH_Qt_array_string
{
    Q_OBJECT

    int uiIndex_;
    LH_QtSetupItem *ui_;
    bool updatingUI_;
    lh_setup_type ui_type_;

public:
    LH_Qt_array_string_ui( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0, lh_setup_type ui_type = lh_type_string)
        : LH_Qt_array_string( parent, ident, size, flags | LH_FLAG_HIDDEN )
    {
        ui_type_ = ui_type;
        ui_ = NULL;
        if(ui_type_ == lh_type_string)
            ui_ = new LH_Qt_QString(parent, QString("%1__ui__").arg(ident).toUtf8(),"", LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | flags );
        if(ui_type_ == lh_type_string_filename)
            ui_ = new LH_Qt_QFileInfo(parent, QString("%1__ui__").arg(ident).toUtf8(),QFileInfo(), LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE | flags );

        Q_ASSERT(ui_ != NULL);

        ui_->setTitle(ident);
        uiIndex_ = 0;
        updatingUI_ = false;

        connect(this, SIGNAL(changed()), this, SLOT(arrayValuesChanged()));
        connect(this, SIGNAL(set()), this, SLOT(arrayValuesChanged()));
        connect(ui_, SIGNAL(changed()), this, SLOT(uiValueChanged()));
    }


    void setEditIndex(int index) {
        uiIndex_ = index;
        arrayValuesChanged();
    }

    void setFlag(int f, bool state)
    {
        if(f != LH_FLAG_HIDDEN)
            LH_Qt_array_string::setFlag(f, state);
        ui_->setFlag(f, state);
    }

    virtual void setHelp(const QString& s)
    {
        LH_Qt_array_string::setHelp(s);
        ui_->setHelp(s);
    }

    void setTitle(const char *s = 0)
    {
        LH_Qt_array_string::setTitle(s);
        ui_->setTitle(s);
    }

    void setTitle( const QString &s )
    {
        LH_Qt_array_string::setTitle(s);
        ui_->setTitle(s);
    }

public slots:
    void arrayValuesChanged() {
        if (uiIndex_>=0 && uiIndex_ < this->size())
        {
            updatingUI_ = true;
            if(ui_type_ == lh_type_string)
                ((LH_Qt_QString*)ui_)->setValue(this->at(uiIndex_));
            if(ui_type_ == lh_type_string_filename)
                ((LH_Qt_QFileInfo*)ui_)->setValue(QFileInfo());
            ui_->refreshData();
            updatingUI_ = false;
        }
    }

    void uiValueChanged(){
        if (updatingUI_) return;
        if (uiIndex_>=0 && uiIndex_ < this->size())
        {
            if(ui_type_ == lh_type_string)
                this->setAt(uiIndex_, ((LH_Qt_QString*)ui_)->value());
            if(ui_type_ == lh_type_string_filename)
                this->setAt(uiIndex_, ((LH_Qt_QFileInfo*)ui_)->value().filePath());

        }
    }

};

#endif // LH_QT_ARRAY_STRING_UI_H
