#ifndef LH_QT_ARRAY_DOUBLE_H
#define LH_QT_ARRAY_DOUBLE_H

#include "LH_Qt_array.h"

class LH_Qt_array_double : public LH_Qt_array
{
public:
    LH_Qt_array_double( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0 )
        : LH_Qt_array( parent, ident, flags, lh_type_array_double )
    {
        resize( size, 0.0 );
    }

    const double *data() const
    {
        return (const double*)item_.data.b.p;
    }

    double *data()
    {
        return (double*)item_.data.b.p;
    }

    int size() const
    {
        Q_ASSERT( (LH_Qt_array::size() % sizeof(double)) == 0 );
        return LH_Qt_array::size() / sizeof(double);
    }

    void fill( int pos = 0, double v = 0 )
    {
        for( int i=pos; i<size(); ++i ) data()[i] = v;
        refreshData();
        emit set();
    }

    void resize( int new_size, double defaultValue = 0 )
    {
        int old_size = size();
        LH_Qt_array::resize( new_size * sizeof(double) );
        fill( old_size, defaultValue );
    }

    double min() const { return item_.param.d.min; }
    double max() const { return item_.param.d.max; }
    double at(int index, double defaultValue = 0.0 ) const
    {
        if( index < 0 || index >= size() ) return defaultValue;
        return data()[index];
    }

    void setAt( int index, double value )
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

#endif // LH_QT_ARRAY_DOUBLE_H
