#ifndef LH_QT_ARRAY_STRING_H
#define LH_QT_ARRAY_STRING_H

#include <QString>
#include <QStringList>
#include "LH_Qt_array.h"

class LH_Qt_array_string : public LH_Qt_array
{
public:
    LH_Qt_array_string( LH_QtObject *parent, const char *ident, const QStringList& value = QStringList(), int flags = lh_meta_default )
        : LH_Qt_array( parent, ident, flags )
    {
        setValue( value );
    }

    QString at( int index, const QString& defaultValue = QString() ) const
    {
        return LH_Qt_array::at(index,defaultValue).toString();
    }

    const QStringList value() const
    {
        return qVariantValue<QStringList>(value());
    }
};

#endif // LH_QT_ARRAY_STRING_H
