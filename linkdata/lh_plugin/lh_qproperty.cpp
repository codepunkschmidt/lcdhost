
#include <QObject>
#include "lh_qproperty.h"

void lh::property::set(QObject& obj) const
{
    const char *name = 0;
    switch( lh_meta_role(meta_) )
    {
    case lh_role_title: name = "title"; break;
    case lh_role_dir: name = "dir"; break;
    case lh_role_help: name = "help"; break;
    case lh_role_link: name = "link"; break;
    case lh_role_sort: name = "sort"; break;
    case lh_role_enabled: name = "enabled"; break;
    case lh_role_visible: name = "visible"; break;
    case lh_role_value: name = "value"; break;
    case lh_role_minimum: name = "minimum"; break;
    case lh_role_maximum: name = "maximum"; break;
    case lh_role_list: name = "list"; break;
    default:
        Q_ASSERT(0);
        return;
    }

    obj.setProperty( name, *this );
    return;
}

QVariant lh_qvariant_from_property( lh_meta meta, const void *data )
{
    const lh_buffer *buf;

    if( data == 0 ) return QVariant();

    switch( lh_meta_format(meta) )
    {
    case lh_format_integer: return qVariantFromValue( *(const int*)data );
    case lh_format_longlong: return qVariantFromValue( *(const long long*)data );
    case lh_format_boolean: return qVariantFromValue( *(const int*)data ? true : false );
    case lh_format_color: return qVariantFromValue( QColor::fromRgba(*(const int*)data) );
    case lh_format_double: return qVariantFromValue( *(const double*)data );
    case lh_format_local8:
        return qVariantFromValue( QString::fromLocal8Bit( (const char *)data ) );
    case lh_format_utf8:
        if( buf = (const lh_buffer *)data, buf->p && buf->n >= 0 )
            return qVariantFromValue( QString::fromUtf8(buf->p,buf->n) );
        break;
    case lh_format_list:
        if( buf = (const lh_buffer *)data, buf->p && buf->n >= 0 )
        {
            const lh_property *prop = (const lh_property *) buf->p;
            int count = buf->n/sizeof(lh_property);
            QVariantList vl;
            while( count -- )
            {
                vl.append( lh_qvariant_from_property( prop->meta, prop->data ) );
                ++ prop;
            }
            return qVariantFromValue( vl );
        }
        break;
    case lh_format_font:
        if( buf = (const lh_buffer *)data, buf->p && buf->n >= 0 )
        {
            QFont f;
            f.fromString( QString::fromUtf8(buf->p,buf->n) );
            return qVariantFromValue( f );
        }
        break;

    case lh_format_png:
        if( buf = (const lh_buffer *)data, buf->p && buf->n >= 0 )
            return qVariantFromValue( QImage::fromData((const uchar*)(buf->p),buf->n) );
        break;

    case lh_format_input:
        return qVariantFromValue( *(const lh_input*)data );

    case lh_format_qvariant:
        return *(const QVariant*)data;

    case lh_format_none:
        return QVariant();

    case lh_format_unused:
        break;
    }

    qCritical() << "unhandled format or conversion" << lh_data_format_to_name(meta);
    return QVariant();
}

void lh_qvariant_to_setup_data( const QVariant& v, int info, lh_property_data& data )
{
    memset( &data, 0, sizeof(lh_property_data) );
    return;
}
