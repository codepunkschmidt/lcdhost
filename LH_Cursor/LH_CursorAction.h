/**
  \file     LH_CursorAction.h
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010 Andy Bridges
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

  */

#ifndef LH_CURSORACTION_H
#define LH_CURSORACTION_H

#include "../LH_QtInstance.h"
#include "../LH_Qt_QString.h"
#include "../LH_Qt_QStringList.h"
#include "../LH_Qt_InputState.h"
#include "../LH_Qt_QTextEdit.h"
#include "../LH_Qt_QFileInfo.h"
#include "../LH_Qt_int.h"
#include "../LH_Qt_bool.h"

#include "LH_CursorData.h"
#include "actionType.h"

#include <QHash>
#include <QList>
#include <QtXml>


class actionTypes
{
    QHash<QString,actionType> actionTypes_;
    QStringList actionTypeIDs_;
    LH_CursorAction *cursorAction_;

    void add(actionType at)
    {
        actionTypes_.insert(at.typeCode, at);
        actionTypeIDs_.append(at.typeCode);
        // return actionTypes_[at.typeCode];
        return;
    }

public:
    actionTypes( LH_CursorAction *p ) : cursorAction_(p) {
        add( actionType(p,"open"      ,"Open Layout"               , QList<actionParameter>() << actionParameter("Layout File",aptFile) ) );
        add( actionType(p,"run"       ,"Run Application"           , QList<actionParameter>() << actionParameter("Application",aptFile,"path") << actionParameter("Parameters",aptString,"args") ) );
        add( actionType(p,"url"       ,"Open URL"                  , QList<actionParameter>() << actionParameter("URL",aptString) ) );
        add( actionType(p,"move"      ,"Move Cursor"               , QList<actionParameter>() << actionParameter("New X Coordinate",aptInteger,"","x",altCursorX) << actionParameter("New Y Coordinate",aptInteger,"","y",altCursorY)) );
        add( actionType(p,"select"    ,"Move Cursor & Select"      , QList<actionParameter>() << actionParameter("New X Coordinate",aptInteger,"","x",altCursorX) << actionParameter("New Y Coordinate",aptInteger,"","y",altCursorY)) );
        add( actionType(p,"wait"      ,"Wait"                      , QList<actionParameter>() << actionParameter("Delay (in ms)",aptInteger,"","",altWait)) );
        add( actionType(p,"deselect"  ,"Clear Selection") );
        add( actionType(p,"deactivate","Deactivate the Cursor") );
        add( actionType(p,"reselect"  ,"Reselect the previous item") );
    }
    actionType at(int index)
    {
        return at(actionTypeIDs_[index]);
    }
    actionType at(QString typeCode)
    {
        return actionTypes_.value(typeCode,actionType(this->cursorAction_));
    }
    QStringList list()
    {
        QStringList list_ = QStringList();
        for (int i=0; i<actionTypeIDs_.count(); i++)
            list_.append(at(i).description);
        return list_;
    }
    int indexOf(QString typeCode)
    {
        return actionTypeIDs_.indexOf(typeCode);
    }
};

class LH_CursorAction : public LH_QtInstance
{
    Q_OBJECT
    actionTypes actionTypes_;

    bool updateState();

    bool fired;
    bool selected;
    bool waiting;
    int lastStep;
    int delay;

    int cludgeLock;

    void enableEditUI(bool);
protected:
    QString statusCode_;

    LH_Qt_QString *setup_coordinate_;
    LH_Qt_InputState *setup_jump_to_;


    LH_Qt_QString     *setup_action_desc_;
    LH_Qt_QStringList *setup_action_type_;
    LH_Qt_bool        *setup_action_enabled_;

    LH_Qt_QString   *setup_action_parameter1_desc_;
    LH_Qt_QString   *setup_action_parameter1_str_;
    LH_Qt_int       *setup_action_parameter1_int_;
    LH_Qt_QFileInfo *setup_action_parameter1_file_;

    LH_Qt_QString   *setup_action_parameter2_desc_;
    LH_Qt_QString   *setup_action_parameter2_str_;
    LH_Qt_int       *setup_action_parameter2_int_;
    LH_Qt_QFileInfo *setup_action_parameter2_file_;

    LH_Qt_QStringList *setup_act_rules_;
    LH_Qt_QString     *setup_act_new_;
    LH_Qt_QString     *setup_act_save_;
    LH_Qt_QString     *setup_act_cancel_;
    LH_Qt_QString     *setup_act_delete_;
    LH_Qt_QString     *setup_act_move_up_;
    LH_Qt_QString     *setup_act_move_down_;
    LH_Qt_QString     *setup_act_copy_;
    LH_Qt_QString     *setup_act_paste_;

    LH_Qt_QTextEdit   *setup_act_XML_;

public:
    LH_CursorAction();
    const char *userInit();

    int polling();

    static lh_class *classInfo();

    void fire(int = 0);

public slots:
    void doJumpTo(QString key,int flags,int value);
    void xmlChanged();
    void reloadAction();
    void saveAction();
    void moveAction_up();
    void moveAction_down();
    void newAction();
    void deleteAction();
    void actionTypeChanged();
    void copyActions();
    void pasteActions();
    void uneditAction();
};

#endif // LH_CURSORACTION_H
