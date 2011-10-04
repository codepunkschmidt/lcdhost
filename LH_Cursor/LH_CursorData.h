#ifndef LH_CURSORDATA_H
#define LH_CURSORDATA_H

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

extern cursorData cursor_data;

#endif // LH_CURSORDATA_H
