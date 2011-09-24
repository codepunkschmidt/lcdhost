#ifndef LH_QT_ARRAY_H
#define LH_QT_ARRAY_H

#include "LH_QtSetupItem.h"

class LH_Qt_array : public LH_QtSetupItem
{
public:
    LH_Qt_array( LH_QtObject *parent, const char *ident, int metainfo = lh_meta_default )
        : LH_QtSetupItem( parent, ident, lh::val(QVariantList()), metainfo )
    { }

    int size() const
    {
        return value().toList().size();
    }

    void resize( int newsize, const QVariant& v = QVariant() )
    {
        QVariantList vl = value().toList();
        while( vl.size() < newsize ) vl.append(v);
        while( vl.size() > newsize ) vl.removeLast();
        setValue(vl);
    }

    QVariant at( int index, const QVariant& def = QVariant() ) const
    {
        QVariantList vl = value().toList();
        if( index < 0 || index >= vl.size() ) return def;
        return vl.at(index);
    }

    void setAt( int index, const QVariant& v )
    {
        QVariantList vl = value().toList();
        if( index < 0 || index >= vl.size() ) return;
        vl[index] = v;
        setValue(vl);
    }

    long long intMin() const { return minimum().toLongLong(); }
    long long intMax() const { return maximum().toLongLong(); }

    double doubleMin() const { return minimum().toDouble(); }
    double doubleMax() const { return maximum().toDouble(); }

    long long intAt(int index) const { return at(index).toLongLong(); }
    double doubleAt(int index) const { return at(index).toDouble(); }
    QString stringAt(int index) const { return at(index).toString(); }
};

#endif // LH_QT_ARRAY_H
