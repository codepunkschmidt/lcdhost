#ifndef LH_CURSORINSTANCE_H
#define LH_CURSORINSTANCE_H

#include "../LH_QtInstance.h"

struct minmax {
    int min;
    int max;
};

struct bounds {
    minmax x;
    minmax y;
};

struct cursorData
{
    int x;
    int y;
    bool active;
    int selX;
    int selY;
    bool selState;
    int lastSelX;
    int lastSelY;
    bool lastSelSet;
    bool sendSelect;
    int lastSelX2;
    int lastSelY2;
    bounds range;
};

class LH_CursorInstance : public LH_QtInstance
{
protected:
    void getCursorData( cursorData& ) const;
    void setCursorData( const cursorData& ) const;
};

#endif // LH_CURSORINSTANCE_H
