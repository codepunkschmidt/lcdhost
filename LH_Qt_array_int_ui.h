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

    bool fixedRange_;

    void init(lh_setup_type ui_type, int uiFlags, bool _fixedRange = false, qint64 min = 0, qint64 max = 0 );

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

    void setEditIndex(int index);

    void setFlag(int f, bool state);

    void setHelp(const QString& s);

    void setTitle(const char *s = 0);

    void setTitle( const QString &s );

    void setMin( double );
    void setMax( double );
    void setMinMax( double, double );

    void setMin( qint64 );
    void setMax( qint64 );
    void setMinMax( qint64, qint64 );

    void setMin( int n ) { setMin( (qint64)n ); }
    void setMax( int n ) { setMax( (qint64)n ); }
    void setMinMax( int a, int b ) { setMinMax( (qint64)a, (qint64)b ); }

public slots:
    void arrayValuesChanged();
    void uiValueChanged();

};

#endif // LH_QT_ARRAY_INT_UI_H
