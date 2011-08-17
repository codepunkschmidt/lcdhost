
#include "LH_QtPlugin.h"
#include "LH_QtLayoutClass.h"
#include "LH_QtInstance.h"

#define RECAST(obj) reinterpret_cast<LH_QtLayoutClass*>(obj)

static lh_layout_item *obj_layout_item_create( lh_layout_class *lc )
{
    return RECAST(lc->obj.ref)->layout_item_create();
}

static void obj_layout_item_destroy( lh_layout_class *lc, lh_layout_item *li )
{
    return RECAST(lc->obj.ref)->layout_item_destroy(li);
}

LH_QtLayoutClass::LH_QtLayoutClass(
    const char *ident,
    const char *title,
    const char *path,
    int width,
    int height,
    LH_QtObject *parent
    ) :
    LH_QtObject( &lc_.obj, ident, parent ),
    path_array_(path)
{
    setTitle( title );
    lc_.size = sizeof(lh_layout_class);
    lc_.path = path_array_.constData();
    lc_.width = width;
    lc_.height = height;
    lc_.obj_layout_item_create = obj_layout_item_create;
    lc_.obj_layout_item_destroy = obj_layout_item_destroy;
    parent->callback( lh_cb_class_create, &lc_ );
}

LH_QtLayoutClass::LH_QtLayoutClass( const lh_class *cls, LH_QtObject *parent ) :
    LH_QtObject( &lc_.obj, cls->ident, parent ),
    path_array_(cls->path)
{
    setTitle( cls->title );
    lc_.size = sizeof(lh_layout_class);
    lc_.path = path_array_.constData();
    lc_.width = cls->width;
    lc_.height = cls->height;
    lc_.obj_layout_item_create = obj_layout_item_create;
    lc_.obj_layout_item_destroy = obj_layout_item_destroy;
    parent->callback( lh_cb_class_create, &lc_ );
}

LH_QtLayoutClass::~LH_QtLayoutClass()
{
    callback( lh_cb_destroy );
}

void LH_QtLayoutClass::layout_item_destroy(lh_layout_item *li)
{
    delete reinterpret_cast<LH_QtInstance*>(li->obj.ref);
}
