#ifndef LH_QT_HTML_H
#define LH_QT_HTML_H

#include "LH_Qt_QString.h"

class LH_Qt_html : public LH_Qt_QString
{
public:
    LH_Qt_html( LH_QtObject *parent, const char *htmltext, int flags = lh_meta_default ) :
        LH_Qt_QString( parent, 0, QString(), flags, lh_type_string_htmllink )
    {
        setOther( htmltext );
        return;
    }
};

#endif // LH_QT_HTML_H
