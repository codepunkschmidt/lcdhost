#ifndef LH_TYPE_MAP_H
#define LH_TYPE_MAP_H

#include <LH/Type.h>
#include <LH/Type_Info.h>
#include <QMetaType>

namespace LH {
namespace Type {

#define LH_TYPE_MAP \
    LH_TYPE_PAIR(bool, Bool) \
    LH_TYPE_PAIR(int, Int) \
    LH_TYPE_PAIR(uint, UInt) \
    LH_TYPE_PAIR(qlonglong, LongLong) \
    LH_TYPE_PAIR(qulonglong, ULongLong) \
    LH_TYPE_PAIR(double, Double) \
    LH_TYPE_PAIR(QChar, QChar) \
    LH_TYPE_PAIR(QString, String) \
    LH_TYPE_PAIR(QStringList, StringList) \
    LH_TYPE_PAIR(QByteArray, ByteArray) \
    LH_TYPE_PAIR(QBitArray, BitArray) \
    LH_TYPE_PAIR(QDate, Date) \
    LH_TYPE_PAIR(QTime, Time) \
    LH_TYPE_PAIR(QDateTime, DateTime) \
    LH_TYPE_PAIR(QUrl, Url) \
    LH_TYPE_PAIR(QLocale, Locale) \
    LH_TYPE_PAIR(QRect, Rect) \
    LH_TYPE_PAIR(QRectF, RectF) \
    LH_TYPE_PAIR(QSize, Size) \
    LH_TYPE_PAIR(QSizeF, SizeF) \
    LH_TYPE_PAIR(QLine, Line) \
    LH_TYPE_PAIR(QLineF, LineF) \
    LH_TYPE_PAIR(QPoint, Point) \
    LH_TYPE_PAIR(QPointF, PointF) \
    LH_TYPE_PAIR(QRegExp, RegExp) \
    LH_TYPE_PAIR(QEasingCurve, EasingCurve) \
    LH_TYPE_PAIR(QFont, Font) \
    LH_TYPE_PAIR(QPixmap, Pixmap) \
    LH_TYPE_PAIR(QBrush, Brush) \
    LH_TYPE_PAIR(QColor, Color) \
    LH_TYPE_PAIR(QPalette, Palette) \
    LH_TYPE_PAIR(QIcon, Icon) \
    LH_TYPE_PAIR(QImage, Image) \
    LH_TYPE_PAIR(QPolygon, Polygon) \
    LH_TYPE_PAIR(QRegion, Region) \
    LH_TYPE_PAIR(QBitmap, Bitmap) \
    LH_TYPE_PAIR(QCursor, Cursor) \
    LH_TYPE_PAIR(QSizePolicy, SizePolicy) \
    LH_TYPE_PAIR(QKeySequence, KeySequence) \
    LH_TYPE_PAIR(QPen, Pen) \
    LH_TYPE_PAIR(QTextLength, TextLength) \
    LH_TYPE_PAIR(QTextFormat, TextFormat) \
    LH_TYPE_PAIR(QMatrix, Matrix) \
    LH_TYPE_PAIR(QTransform, Transform) \
    LH_TYPE_PAIR(QMatrix4x4, Matrix4x4) \
    LH_TYPE_PAIR(QVector2D, Vector2D) \
    LH_TYPE_PAIR(QVector3D, Vector3D) \
    LH_TYPE_PAIR(QVector4D, Vector4D) \
    LH_TYPE_PAIR(QQuaternion, Quaternion) \
    LH_TYPE_PAIR(void*, VoidStar) \
    LH_TYPE_PAIR(long, Long) \
    LH_TYPE_PAIR(short, Short) \
    LH_TYPE_PAIR(char, Char) \
    LH_TYPE_PAIR(ulong, ULong) \
    LH_TYPE_PAIR(ushort, UShort) \
    LH_TYPE_PAIR(uchar, UChar) \
    LH_TYPE_PAIR(float, Float) \
    LH_TYPE_PAIR(QObject*, ObjectStar) \
    LH_TYPE_PAIR(QWidget*, WidgetStar) \
    LH_TYPE_PAIR(QVariant, Variant)

template < typename SELFTYPE, typename DATATYPE, template <typename,typename> class OPERATION >
static bool Map( SELFTYPE & self, int type, DATATYPE * data )
{
    switch( type )
    {
#define LH_TYPE_PAIR( OTHERTYPE, NAME ) \
    case QMetaTypeId2< OTHERTYPE >::MetaType : \
        return OPERATION< SELFTYPE, OTHERTYPE >::call( self, (OTHERTYPE*) data );
    LH_TYPE_MAP
#undef LH_TYPE_PAIR
    default: break;
    }
    qCritical( "LH::Type::Map: unhandled type %d=%s", type, QMetaType::typeName(type) );
    return false;
}

} // namespace Type
} // namespace LH

#endif // LH_TYPE_MAP_H
