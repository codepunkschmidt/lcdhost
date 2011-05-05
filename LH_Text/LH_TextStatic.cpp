
#include <QImage>
#include <QFont>
#include <QFontMetrics>

#include "LH_Text.h"

class LH_TextStatic : public LH_Text
{
public:
    LH_TextStatic(const char *name) : LH_Text(name) {}

    static lh_class *classInfo()
    {
        static lh_class classinfo =
        {
            sizeof(lh_class),
            "Static",
            "StaticText",
            "Text",
            -1, -1,
            lh_instance_calltable_NULL
        };

        if( classinfo.width == -1 )
        {
            QImage tmpimage = makeImage();
            QFont font("Arial",10);
            QFontMetrics fm( font, &tmpimage );
            classinfo.height = fm.height();
            classinfo.width = fm.width("LCDHost");
        }

        return &classinfo;
    }
};

LH_PLUGIN_CLASS(LH_TextStatic)
