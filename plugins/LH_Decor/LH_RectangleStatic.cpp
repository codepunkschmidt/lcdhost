#include "LH_Rectangle.h"

class LH_RectangleStatic : public LH_Rectangle
{
public:
    static lh_class *classInfo()
    {
        static lh_class classInfo =
        {
            sizeof(lh_class),
            "Static",
            "StaticRectangle",
            "Rounded rectangle",
            48,48,
            lh_object_calltable_NULL,
            lh_instance_calltable_NULL
        };

        return &classInfo;
    }
};

LH_PLUGIN_CLASS(LH_RectangleStatic)
