#ifndef LH_QPROPERTY_H
#define LH_QPROPERTY_H

#include "lh_qvariant.h"

class QObject;

void lh_set_qproperty( const lh_meta meta, const void *data, QObject& );
lh_meta lh_meta_from_qproperty( const QObject&, const lh_meta_role role );
lh_meta lh_meta_from_qproperty( const QObject&, const char *name );

/**
  Constructs a lh_property from a QVariant. Obeys the meta
  format if given. The caller needs to free the lh_property
  with lh_property_free() once done with it.
  */
lh_property *lh_property_from_qvariant( const QVariant&, lh_meta meta = lh_format_none );

/**
  These are syntactic sugar to allow setting properties in constructors
  and passing properties as parameters in general.
  */
namespace lh
{
    class property : public QVariant
    {
    protected:
        const int meta_;

    public:
        inline property( const QVariant& value, const int meta = lh_meta_default ) :
            QVariant( value ), meta_(meta)
        { }
        property( const QObject& obj, const char *name );
        property( const QObject& obj, const lh_meta_role role );
        void set(QObject& obj) const;
    };

    class val : public property
    {
    public:
        inline val( const QVariant& value, const int meta = lh_meta_default ) :
            property( value, lh_role_value|meta )
        { }
    };
}

#endif // LH_QPROPERTY_H
