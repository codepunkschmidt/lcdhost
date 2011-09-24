#ifndef LH_QTLAYOUTCLASS_H
#define LH_QTLAYOUTCLASS_H

#include <QByteArray>
#include "LH_QtObject.h"

// structure used when using the layout class auto-registration
typedef struct lh_class_t
{
    int size; /* sizeof(lh_class) */
    const char *path;
    const char *ident;
    const char *title;
    int width;
    int height;
} lh_class;

class LH_QtLayoutClass : public LH_QtObject
{
    Q_OBJECT
    QByteArray path_array_;
    lh_layout_class lc_;

public:
    explicit LH_QtLayoutClass(
        const char *ident,
        const char *title,
        const char *path,
        int width,
        int height,
        LH_QtObject *parent );
    LH_QtLayoutClass( const lh_class *cls, LH_QtObject *parent );
    ~LH_QtLayoutClass();

    LH_QtObject *parent() const { return static_cast<LH_QtObject *>(LH_QtObject::parent()); }

    const char *path() const { return lc_.path; }
    int width() const { return lc_.width; }
    int height() const { return lc_.height; }

    virtual lh_layout_item *layout_item_create() { return 0; }
    virtual void layout_item_destroy(lh_layout_item *li);
};

#endif // LH_QTLAYOUTCLASS_H
