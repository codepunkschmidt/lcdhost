
#include <QDebug>
#include <QByteArrayMatcher>
#include "LH_Qt_array.h"

int LH_Qt_array::size() const
{
    Q_ASSERT( type() & lh_format_list );
    if( type() & lh_format_integer )
    {
        Q_ASSERT( item_.data.value.b.n % sizeof(long long) == 0 );
        return item_.data.value.b.n / sizeof(long long);
    }
    if( type() & lh_format_double )
    {
        Q_ASSERT( item_.data.value.b.n % sizeof(double) == 0 );
        return item_.data.value.b.n / sizeof(double);
    }
    if( type() & lh_format_string )
    {
        if( item_.data.value.b.p == 0 || item_.data.value.b.n<1 ) return 0;
        char utf8_line_separator[3] = { 0xE2, 0x80, 0xA9 };
        return QByteArray::fromRawData(item_.data.value.b.p,item_.data.value.b.n)
                .count( QByteArray::fromRawData(utf8_line_separator,3) ) + 1;
    }
    return item_.data.value.b.n;
}

void LH_Qt_array::fill( const QVariant& v, int fromindex )
{
    Q_ASSERT( type() & lh_format_list );

    int currsize = size();

    if( fromindex<0 || fromindex>=currsize ) return;

    if( type() & lh_format_integer )
    {
        long long ll = v.toLongLong();
        for( int i=fromindex; i<currsize; i++ ) ((long long *)item_.data.value.b.p)[i] = ll;
        refreshValue();
        return;
    }

    if( type() & lh_format_double )
    {
        double d = v.toDouble();
        for( int i=fromindex; i<currsize; i++ ) ((double *)item_.data.value.b.p)[i] = d;
        refreshValue();
        return;
    }

    if( type() & lh_format_string )
    {
        QStringList sl( QString::fromUtf8(item_.data.value.b.p,item_.data.value.b.n).split(QChar(QChar::LineSeparator)) );
        QString s = v.toString();
        for( int i=fromindex; i<currsize; i++ ) sl[i] = s;
        setValueArray( sl.join(QChar(QChar::LineSeparator)).toUtf8() );
        refreshValue();
        return;
    }

    char c = (char) v.toInt();
    for( int i=fromindex; i<currsize; i++ ) item_.data.value.b.p[i] = c;
    refreshValue();
    return;
}

void LH_Qt_array::resize( int newsize, const QVariant& v )
{
    Q_ASSERT( type() & lh_format_list );
    if( newsize < 0 ) return;
    int oldsize = size();
    if( oldsize == newsize ) return;

    if( type() & lh_format_integer )
    {
        value_resize( newsize * sizeof(long long) );
        if( newsize > oldsize ) fill( v, oldsize );
        return;
    }

    if( type() & lh_format_double )
    {
        value_resize( newsize * sizeof(double) );
        if( newsize > oldsize ) fill( v, oldsize );
        return;
    }

    if( type() & lh_format_string )
    {
        QStringList sl;
        if( oldsize ) sl = QString::fromUtf8(item_.data.value.b.p,item_.data.value.b.n).split(QChar(QChar::LineSeparator));
        Q_ASSERT( sl.size() == oldsize );
        QString s = v.toString();
        while( sl.size() < newsize ) sl.append( s );
        while( sl.size() > newsize ) sl.removeLast();
        setValueArray( sl.join(QChar(QChar::LineSeparator)).toUtf8() );
        return;
    }

    value_resize( newsize );
    if( newsize > oldsize ) fill( v, oldsize );
    return;
}

QVariant LH_Qt_array::at( int index ) const
{
    if( index < 0 || item_.data.value.b.p == 0 || item_.data.value.b.n < 1 ) return QVariant();
    if( type() & lh_format_integer )
    {
        if( index >= item_.data.value.b.n/sizeof(long long) ) return QVariant();
        return qVariantFromValue( ((long long *)item_.data.value.b.p)[index] );
    }
    if( type() & lh_format_double )
    {
        if( index >= item_.data.value.b.n/sizeof(double) ) return QVariant();
        return qVariantFromValue( ((double *)item_.data.value.b.p)[index] );
    }
    if( type() & lh_format_string )
    {
        QByteArrayMatcher matcher( "\xE2\x80\xA9", 3 );
        int start = 0;
        while( index > 0 )
        {
            start = matcher.indexIn( valueArray(), start );
            if( start < 0 ) return QVariant();
            start += 3;
            index --;
        }
        int stop = matcher.indexIn( valueArray(), start );
        if( stop < start ) stop = valueArray().size();
        return qVariantFromValue( QString::fromUtf8( item_.data.value.b.p+start, stop-start ) );
    }
    if( index >= item_.data.value.b.n ) return QVariant();
    return qVariantFromValue( (int) item_.data.value.b.p[index] );
}

void LH_Qt_array::setAt( int index, const QVariant& v )
{
    if( index < 0 || item_.data.value.b.p == 0 || item_.data.value.b.n < 1 ) return;

    if( type() & lh_format_integer )
    {
        if( index >= item_.data.value.b.n/sizeof(long long) ) return;
        ((long long *)item_.data.value.b.p)[index] = v.toLongLong();
        refreshValue();
        return;
    }

    if( type() & lh_format_double )
    {
        if( index >= item_.data.value.b.n/sizeof(double) ) return;
        ((double *)item_.data.value.b.p)[index] = v.toDouble();
        refreshValue();
        return;
    }

    if( type() & lh_format_string )
    {
        if( index >= size() ) return;
        QStringList sl( QString::fromUtf8(item_.data.value.b.p,item_.data.value.b.n).split(QChar(QChar::LineSeparator)) );
        sl[index] = v.toString();
        setValueArray( sl.join(QChar(QChar::LineSeparator)).toUtf8() );
        refreshValue();
        return;
    }

    if( index >= item_.data.value.b.n ) return;
    item_.data.value.b.p[index] = (char) v.toInt();
    refreshValue();
    return;
}
