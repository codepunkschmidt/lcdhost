#ifndef LH_QT_ARRAY_H
#define LH_QT_ARRAY_H

#include "LH_QtSetupItem.h"

class LH_Qt_array : public LH_QtSetupItem
{
public:
    LH_Qt_array( LH_QtObject *parent, const char *ident, int flags = 0, lh_setup_type subtype = lh_type_array )
        : LH_QtSetupItem( parent, ident, subtype, flags )
    {
    }

    virtual int size() const
    {
        return data_array_.size();
    }

    void resize( int size )
    {
        if( size > 0 )
        {
            data_array_.resize( size );
            item_.data.b.p = data_array_.data();
            item_.data.b.n = data_array_.size();
        }
        else
        {
            data_array_.clear();
            item_.data.b.p = 0;
            item_.data.b.n = 0;
        }
    }

    virtual qint64 intMin() const { return 0; }
    virtual qint64 intMax() const { return 0; }
    virtual qint64 intAt(int) const { return 0; }
    virtual double doubleMin() const { return 0.0; }
    virtual double doubleMax() const { return 0.0; }
    virtual double doubleAt(int) const { return 0.0; }
    virtual QString stringAt(int) const { return QString(); }
};

#endif // LH_QT_ARRAY_H
