#ifndef LH_QT_ARRAY_DOUBLE_H
#define LH_QT_ARRAY_DOUBLE_H

#include "LH_Qt_array.h"

class LH_Qt_array_double : public LH_Qt_array
{
public:
    LH_Qt_array_double( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0 )
        : LH_Qt_array( parent, ident, size, flags, lh_type_array_double )
    {
    }

    double min() const { return item_.param.d.min; }
    double max() const { return item_.param.d.max; }

    double at(int index) const
    {
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < size() );
        return ((double*)item_.data.b.p)[index];
    }

    double at(int index, double defaultValue) const
    {
        if( index < 0 || index >= size() ) return defaultValue;
        return ((double*)item_.data.b.p)[index];
    }

    void setAt( int index, double value )
    {
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < size() );
        ((double*)item_.data.b.p)[index] = value;
        emit(set());
    }

    void resize(int newSize, double defaultValue)
    {
        doubleResize(newSize, defaultValue);
    }
};

#endif // LH_QT_ARRAY_DOUBLE_H
