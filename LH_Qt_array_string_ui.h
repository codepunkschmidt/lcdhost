#ifndef LH_QT_ARRAY_STRING_UI_H
#define LH_QT_ARRAY_STRING_UI_H

#include "LH_Qt_array_string.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_QFileInfo.h"
#include <QDebug>

class LH_Qt_array_string_ui : public LH_Qt_array_string
{
    Q_OBJECT

    LH_Qt_QString *ui_;
    int uiIndex_;

    void init(lh_setup_type ui_type, int uiFlags, const QStringList& listItems );

public:
    LH_Qt_array_string_ui( LH_QtObject *parent, const char *ident,
                           const QStringList& value = QStringList(),
                           int flags = 0, lh_setup_type ui_type = lh_type_string,
                           const QStringList& listItems = QStringList() )
        : LH_Qt_array_string( parent, ident, value, flags | LH_FLAG_HIDDEN ),
          ui_(0), uiIndex_(0)
    {
        init( ui_type, LH_FLAG_NOSAVE|LH_FLAG_NOSINK|LH_FLAG_NOSOURCE|flags, listItems );
    }

    void setEditIndex(int index);
    int indexAt(int i);

    LH_Qt_QString* ui() { return ui_; }

    int currentIndex();
    void setCurrentIndex(int i);
    void setFlag(int f, bool state);

public slots:
    void arrayValuesChanged();
    void uiValueChanged();
};

#endif // LH_QT_ARRAY_STRING_UI_H
