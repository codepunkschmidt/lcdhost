#ifndef LH_QT_ARRAY_STRING_H
#define LH_QT_ARRAY_STRING_H

#include "LH_Qt_array.h"
#include <QString>

class LH_Qt_array_string : public LH_QtSetupItem
{
    QStringList list_;

public:
    LH_Qt_array_string( LH_QtObject *parent, const char *ident, int size = 0, int flags = 0 )
        : LH_QtSetupItem( parent, ident, lh_type_array_string, flags ), list_()
    {
        resize( size );
    }

    void setup_change()
    {
        list_ = QString::fromUtf8( (const char*) item_.data.b.p ).split(QChar(0));
        LH_QtSetupItem::setup_change();
    }

    void resize(int size)
    {
        Q_ASSERT( item_.type & lh_type_array );
        Q_ASSERT( size >= 0 );
        if( item_.type == lh_type_array_string )
        {
            while ( list_.count() > size )
                list_.removeLast();
            while ( list_.count() < size )
                list_.append("");
            setArray(list_.join(QChar(0)).toUtf8());
        }
    }

    int size() const
    {
        if( item_.type == lh_type_array_string )
        {
            return list_.count();
        }
        return 0;
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
