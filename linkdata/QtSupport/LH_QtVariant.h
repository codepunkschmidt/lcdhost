#ifndef LH_QTVARIANT_H
#define LH_QTVARIANT_H

#include <QObject>
#include "LH_QVariant.h"

class LH_QtObject;

/**
  Wraps a lh_variant with a LH_QVariant and handles
  updates to and from it. If you don't set a parent
  LH_QtObject, it won't be able to resize the data
  area.
  */

class LH_QtVariant : public QObject, public QVariant
{
    lh_variant& lhv_;

public:
    LH_QtVariant( lh_variant& lhv, LH_QtObject *parent = 0 );
    ~LH_QtVariant() { memset(&lhv_,0,sizeof(lh_variant)); }

    LH_QtObject *parent() const { return reinterpret_cast<LH_QtObject *>(QObject::parent()); }

    const lh_variant& lhv() const { return lhv_; }
    lh_format format() const { return lhv().fmt; }

    void setFormat( lh_format fmt );
    void read();
    void write() const;
};

#endif // LH_QTVARIANT_H
