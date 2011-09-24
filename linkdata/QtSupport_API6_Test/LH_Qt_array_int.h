#ifndef LH_QT_ARRAY_INT_H
#define LH_QT_ARRAY_INT_H

#include "LH_Qt_array.h"

class LH_Qt_array_int : public LH_Qt_array
{
public:
    LH_Qt_array_int( LH_QtObject *parent, const char *ident, int size = 0, int flags = lh_meta_default )
        : LH_Qt_array( parent, ident, flags )
    {
        resize( size );
    }

    int at(int index,int def = 0) const { return LH_Qt_array::at(index,def).toInt(); }
    int min() const { return LH_QtSetupItem::minimum().toInt(); }
    int max() const { return LH_QtSetupItem::maximum().toInt(); }
    void setMin( int v ) { setMinimum(v); }
    void setMax( int v ) { setMaximum(v); }
};

#endif // LH_QT_ARRAY_INT_H
