#ifndef LH_QT_HTML_H
#define LH_QT_HTML_H

#include "LH_QtSetupItem.h"

class LH_Qt_html : public LH_QtSetupItem
{
public:
    LH_Qt_html( LH_QtObject *parent, const char *htmltext, int metainfo = lh_meta_default|lh_ui_htmllink ) :
        LH_QtSetupItem( parent, 0, lh::val(QString()), lh::list(htmltext), metainfo )
    { }
};

#endif // LH_QT_HTML_H
