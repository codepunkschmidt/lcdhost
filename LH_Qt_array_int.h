#ifndef LH_QT_ARRAY_INT_H
#define LH_QT_ARRAY_INT_H

#include "LH_Qt_array.h"

class LH_Qt_array_int : public LH_Qt_array
{
public:
    LH_Qt_array_int( LH_QtObject *parent, const QString& name, int size = 0, int flags = 0 )
        : LH_Qt_array( parent, name, size, flags, lh_type_array_qint64 )
    {
    }

    qint64 min() const { return item_.param.i.min; }
    qint64 max() const { return item_.param.i.max; }

    qint64 at(int index) const
    {
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < size() );
        return ((qint64*)item_.data.b.p)[index];
    }

    int operator[]( int index ) const
    {
        return at(index);
    }

    qint64& operator[]( int index )
    {
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < size() );
        return ((qint64*)item_.data.b.p)[index];
    }
};

#endif // LH_QT_ARRAY_INT_H
