#include "LH_Qt_QString.h"
#include <QDebug>

void LH_Qt_QString::setup_resize( size_t needed )
{
    array_.resize(needed);
    item_.param.size = array_.capacity();
    item_.data.s = array_.data();
    return;
}

void LH_Qt_QString::setup_change()
{
    Q_ASSERT( item_.data.s == array_.constData() );
    str_ = QString::fromUtf8( item_.data.s );
    emit change( str_ );
    LH_QtSetupItem::setup_change();
    return;
}

void LH_Qt_QString::setValue(const QString &s)
{
    Q_ASSERT( item_.data.s == array_.constData() );
    if( s != str_ )
    {
        str_ = s;
        array_ = str_.toUtf8();
        item_.param.size = array_.capacity();
        item_.data.s = array_.data();
        refresh();
        emit set();
    }
}
