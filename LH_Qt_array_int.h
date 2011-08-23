#ifndef LH_QT_ARRAY_INT_H
#define LH_QT_ARRAY_INT_H

#include "LH_Qt_array.h"

class LH_Qt_array_int : public LH_Qt_array
{
public:
    LH_Qt_array_int( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0 )
        : LH_Qt_array( parent, ident, flags, lh_type_array_qint64 )
    {
        resize( size, 0 );
    }

    const qint64 *data() const
    {
        return (const qint64*)item_.data.b.p;
    }

    qint64 *data()
    {
        return (qint64*)item_.data.b.p;
    }

    int size() const
    {
        Q_ASSERT( (LH_Qt_array::size() % sizeof(qint64)) == 0 );
        return LH_Qt_array::size() / sizeof(qint64);
    }

    void fill( int pos = 0, qint64 v = 0 )
    {
        for( int i=pos; i<size(); ++i ) data()[i] = v;
        refreshData();
        emit set();
    }

    void resize( int new_size, qint64 defaultValue = 0 )
    {
        int old_size = size();
        LH_Qt_array::resize( new_size * sizeof(qint64) );
        fill( old_size, defaultValue );
    }

    qint64 min() const { return item_.param.i.min; }
    qint64 max() const { return item_.param.i.max; }
    qint64 at(int index, qint64 defaultValue = 0 ) const
    {
        if( index < 0 || index >= size() ) return defaultValue;
        return data()[index];
    }

    void setAt( int index, qint64 value )
    {
        Q_ASSERT( index >= 0 );
        Q_ASSERT( index < size() );
        data()[index] = value;
        refreshData();
        emit set();
    }

    qint64 intMin() const { return min(); }
    qint64 intMax() const { return max(); }
    qint64 intAt(int index) const { return at(index); }
    double doubleMin() const { return min(); }
    double doubleMax() const { return max(); }
    double doubleAt(int index) const { return at(index); }
    QString stringAt(int index) const { return QString::number(at(index)); }
};

#endif // LH_QT_ARRAY_INT_H
