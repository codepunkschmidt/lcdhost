/**
  \file     LH_CursorController.h
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010,2011 Andy Bridges
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.

 **/

#ifndef LH_CURSORCONTROLLER_H
#define LH_CURSORCONTROLLER_H

// #include <windows.h>
// #include <tchar.h>
// #include <stdio.h>

#include "../LH_Text/LH_Text.h"
#include "../LH_Qt_InputState.h"
#include "../LH_Qt_QFileInfo.h"

#include "LH_CursorData.h"

enum selectMode
{
    smNone,
    smSelect,
    smSelectDeselect
};

struct cursorMode{
    selectMode select;
    bool activate;
    QString description;
};

class LH_CursorController : public LH_QtInstance
{
    Q_OBJECT

    QList<cursorMode> cursorModes;

    void persistSelection();

protected:

    LH_Qt_QString *setup_coordinate_;
    LH_Qt_QStringList *setup_mode_;
    LH_Qt_QString *setup_boundry_;
    LH_Qt_bool *setup_boundry_loop_;
    LH_Qt_QString *setup_invalid_coordinates_;
    LH_Qt_bool *setup_jump_invalid_;
    LH_Qt_InputState *setup_move_up_;
    LH_Qt_InputState *setup_move_down_;
    LH_Qt_InputState *setup_move_left_;
    LH_Qt_InputState *setup_move_right_;
    LH_Qt_InputState *setup_select_;
    LH_Qt_InputState *setup_reselect_;
    LH_Qt_InputState *setup_activate_;

    LH_Qt_bool *setup_persistent_;
    LH_Qt_bool *setup_persistent_autoselect_;
    LH_Qt_QFileInfo *setup_persistent_file_;

public:
    LH_CursorController();

    const char *userInit(){ hide(); return NULL; }

    int polling();

    static lh_class *classInfo();

public slots:
    void doMoveUp(QString key,int flags,int value);
    void doMoveDown(QString key,int flags,int value);
    void doMoveLeft(QString key,int flags,int value);
    void doMoveRight(QString key,int flags,int value);
    void doSelect(QString key,int flags,int value);
    void doReselect(QString key,int flags,int value);
    void doActivate(QString key,int flags,int value);

    void updateLocation(int xMod, int yMod, bool absolute = false);
    void changeMode();
    void changeBounds();

    void changePersistent();
    void loadPersistedSelection();
};

#endif // LH_CURSORCONTROLLER_H
