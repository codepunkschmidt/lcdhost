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
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < list_.count() );
        return list_[index];
    }

    QString at(int index, QString defaultValue) const
    {
        if( index < 0 || index >= list_.count() ) return defaultValue;
        return list_[index];
    }

    void setAt( int index, QString value )
    {
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < list_.count() );
        list_[index] = value;
        setArray(list_.join(QChar(0)).toUtf8());
    }
};

#endif // LH_QT_ARRAY_STRING_H
