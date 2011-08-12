#ifndef LH_QT_ARRAY_STRING_H
#define LH_QT_ARRAY_STRING_H

#include "LH_Qt_array.h"

class LH_Qt_array_string : public LH_Qt_array
{
public:
    LH_Qt_array_string( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0 )
        : LH_Qt_array( parent, ident, size, flags, lh_type_array_string )
    {
    }

    QString at(int index) const
    {
        QStringList _list = QString::fromUtf8( (const char*) item_.data.b.p ).split(QChar(0));
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < _list.length() );
        return _list[index];
    }

    QString at(int index, QString defaultValue) const
    {
        if( index < 0 || index >= size() ) return defaultValue;
        return at(index);
    }

    void setAt( int index, double value )
    {
        QStringList _list = QString::fromUtf8( (const char*) item_.data.b.p ).split(QChar(0));
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < _list.length() );
        _list[index] = value;
        setArray(_list.join(QChar(0)).toUtf8());
    }
};

#endif // LH_QT_ARRAY_STRING_H
