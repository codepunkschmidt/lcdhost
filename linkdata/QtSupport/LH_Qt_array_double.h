#ifndef LH_QT_ARRAY_DOUBLE_H
#define LH_QT_ARRAY_DOUBLE_H

#include "LH_Qt_array.h"

class LH_Qt_array_double : public LH_Qt_array
{
public:
    LH_Qt_array_double( LH_QtObject *parent, const char *ident, int size = 0, int flags = lh_meta_default )
        : LH_Qt_array( parent, ident, flags )
    {
        resize( size, 0.0 );
    }

    double min() const { return LH_Qt_array::minimum().toDouble(); }
    double max() const { return LH_Qt_array::maximum().toDouble(); }

    double at(int index, double defaultValue = 0.0 ) const
    {
        return LH_Qt_array::at(index,defaultValue).toDouble();
    }
};

#endif // LH_QT_ARRAY_DOUBLE_H
