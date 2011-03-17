/**
  \file     LH_CursorController.cpp
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

#include "LH_CursorController.h"

#include "QDebug"

LH_PLUGIN_CLASS(LH_CursorController)

lh_class *LH_CursorController::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorController",
        "Cursor Controller",
        -1, -1,
        lh_instance_calltable_NULL
    };

    return &classInfo;
}

LH_CursorController::LH_CursorController(const char * name) : LH_Text(name), shmem_(NULL), cursor_location_(NULL)
{
    cursorModes.append((cursorMode){smSelectDeselect, true , "Activate, Move & Select / Deselect"});
    cursorModes.append((cursorMode){smSelect        , true , "Activate, Move & Select Only (one is always selected)"});
    cursorModes.append((cursorMode){smNone          , true , "Activate & Move Only (Highlighted option is selected)"});
    cursorModes.append((cursorMode){smNone          , false, "Move Only (Highlighted option is selected, always activated)"});
    cursorModes.append((cursorMode){smSelectDeselect, false, "Move & Select / Deselect Only (Always activated)"});

    shmem_ = new QSharedMemory( "LHCursorSharedMemory", this );
    if( shmem_->create(sizeof(cursorData)) )
    {
        if( shmem_->lock() )
        {
            cursor_location_ = static_cast<cursorData*>( shmem_->data() );
            if( cursor_location_ )
            {
                memset(cursor_location_, '\0', sizeof(cursorData));
                cursor_location_->active = false;
                cursor_location_->x=1;
                cursor_location_->y=1;
                cursor_location_->selX = 0;
                cursor_location_->selY = 0;
                cursor_location_->selState = false;
            }
            shmem_->unlock();
        }
        else qWarning() << "LH_CursorController: failed to lock shared memory";
    }
    else qWarning() << "LH_CursorController: failed to create shared memory";

    setup_text_->setFlag(LH_FLAG_HIDDEN,true);
    setup_font_->setFlag(LH_FLAG_HIDDEN,true);
    setup_bgcolor_->setFlag(LH_FLAG_HIDDEN,true);
    setup_horizontal_->setFlag(LH_FLAG_HIDDEN,true);
    setup_vertical_->setFlag(LH_FLAG_HIDDEN,true);
    setup_scrollrate_->setFlag(LH_FLAG_HIDDEN,true);
    setup_scrollstep_->setFlag(LH_FLAG_HIDDEN,true);
    setup_fontresize_->setFlag(LH_FLAG_HIDDEN,true);
    setup_pencolor_->setFlag(LH_FLAG_HIDDEN,true);



    QStringList modeNames = QStringList();
    foreach(cursorMode cm, cursorModes) modeNames.append(cm.description);
    setup_mode_ = new LH_Qt_QStringList(this,tr("Cursor Mode"),modeNames,LH_FLAG_AUTORENDER);
    setup_mode_->setHelp("This allows the user to select how the cursor should behave. Each cursor mode is a combination of several options:<br/>"
                         "<br/>"
                         "<ul>"
                         "<li><b>Activate</b></br>"
                         "If a mode includes \"activation\" then in it's default state the cursor is hidden and nothing happens when using the movement or selection keys. Insetad the user must first press the activation key to display the cursor. Whilst active the cursor is visible, can be moved and, if appropriate, items can be selected and deselected. If activation is not included in the mode, then the cursor is assumed to always be active.</li>"
                         "<li><b>Select</b></br>"
                         "For modes that include selecting, the user must first move the cursor over the desired option and then press the select key before anything happens. If a mode does not include selection, then simply moving the cursor changes the selection immediately.</li>"
                         "</li>"
                         "<li><b>Deselect</b></br>"
                         "Some modes have select/deselect behaviour, specified by this option. With this ability the user can \"turn off\" everything; without it there will always be one item selected.</li>"
                         "</li>"
                         "</ul>");
    connect( setup_mode_, SIGNAL(changed()), this, SLOT(changeMode()) );

    setup_boundry_ = new LH_Qt_QString(this,"Boundries","1,1,3,3",0);
    setup_boundry_->setHelp("This box should contain the borders of the \"box\" the cusror can move in.</br></br>The format is [left],[top],[right],[bottom] <br/><br/>e.g.:<br/>\"1,1,1,4\" would denote a box one column wide and four rows high<br/>\"1,1,3,3\" would be a square of three columns and three rows.");
    connect( setup_boundry_, SIGNAL(changed()), this, SLOT(changeBounds()) );
    setup_boundry_loop_ = new LH_Qt_bool(this, "^Loop at edges", false);
    setup_boundry_loop_->setHelp("By default when at the furthest edge of the box the cursor stops. Ticking this however will change that so that moving off one edge will bring the cursor back in on the opposite side.");

    setup_invalid_coordinates_ = new LH_Qt_QString(this,"Invalid Points\n(x,y;x,y;x,y)","",0);
    setup_invalid_coordinates_->setHelp("If you wish to create a \"void\" in the cusrsor space, this allows you to do so. List all the points here that the cursor is not allowed to land on and it will be prevented from doing so either by jumping over the invlid points to the next valid one or simply by blocking its movement.<br/><br/>The format is [x1],[y1];[x2],[y2];[x3][y3]; etc...<br/>e.g.: for the boundry \"1,1,2,5\" the invalid points \"1,2;1,3;1,4\" would prevent the cursor landing on any of the top three middle cells, creating a \"U\"-like shape.");
    setup_jump_invalid_ = new LH_Qt_bool(this, "^Jump over invalid points", true);
    setup_jump_invalid_->setHelp("By default when the cursor tries to move onto an invlid point it \"jumps\" the cell in question, and keeps moving in the same direction until reaching one where it is allowed to be. Alternatively, checking this box will prevent this, turning invalid points into \"walls\" that have to be moved around manually.");

    setup_move_up_ = new LH_Qt_InputState(this,"Up","",0);
    setup_move_up_->setHelp("The key used to move the cursor up, usually the up key from the LCD control");
    connect( setup_move_up_, SIGNAL(input(QString,int,int)), this, SLOT(doMoveUp(QString,int,int)) );

    setup_move_down_ = new LH_Qt_InputState(this,"Down","",0);
    setup_move_down_->setHelp("The key used to move the cursor down, usually the down key from the LCD control");
    connect( setup_move_down_, SIGNAL(input(QString,int,int)), this, SLOT(doMoveDown(QString,int,int)) );

    setup_move_left_ = new LH_Qt_InputState(this,"Left","",0);
    setup_move_left_->setHelp("The key used to move the cursor left, usually the left key from the LCD control");
    connect( setup_move_left_, SIGNAL(input(QString,int,int)), this, SLOT(doMoveLeft(QString,int,int)) );

    setup_move_right_ = new LH_Qt_InputState(this,"Right","",0);
    setup_move_right_->setHelp("The key used to move the cursor right, usually the right key from the LCD control");
    connect( setup_move_right_, SIGNAL(input(QString,int,int)), this, SLOT(doMoveRight(QString,int,int)) );

    setup_select_ = new LH_Qt_InputState(this,"Select","",0);
    setup_select_->setHelp("The key used to select the currently highlighted option, usually the Ok key from the LCD control");
    connect( setup_select_, SIGNAL(input(QString,int,int)), this, SLOT(doSelect(QString,int,int)) );

    setup_reselect_ = new LH_Qt_InputState(this,"Reselect","",0);
    setup_reselect_->setHelp("The key used to reselect the previously selected highlighted option. Usually this is unused, but is very useful for menu layouts where it acts as a \"return to last layout\" (or cancel) command");
    connect( setup_reselect_, SIGNAL(input(QString,int,int)), this, SLOT(doReselect(QString,int,int)) );

    setup_activate_ = new LH_Qt_InputState(this,"Enable","",0);
    setup_activate_->setHelp("The key used to activate the cursor control");
    connect( setup_activate_, SIGNAL(input(QString,int,int)), this, SLOT(doActivate(QString,int,int)) );

    setup_persistent_ = new LH_Qt_bool(this,"^Persistent Selection",false,0);
    setup_persistent_->setHelp("Ticking this box will cause the system to automatically recall the cursor's last position whether you save the layout or not.<br/><br/>This is mainly of use within menu layouts and allows the menu to open with the last selected option still highlighted.");
    setup_persistent_autoselect_ = new LH_Qt_bool(this,"^Auto-reselect",false, LH_FLAG_HIDDEN);
    setup_persistent_autoselect_->setHelp("When recovering a persited selection, this option determins if the option is merely highlighed (i.e. the cursor is positioned on it) or if the option is actually selected.");
    setup_persistent_file_ = new LH_Qt_QFileInfo(this, "Persistence File", QFileInfo(), LH_FLAG_HIDDEN);
    setup_persistent_file_->setHelp("The file in which to save the persisted location.<br/><br/>Note that you can use this to make serveral layouts work together as though they were all parts of the same layout by pointing them all to this file, thus causing the cursor position to persist accross them.");

    connect(setup_persistent_, SIGNAL(changed()), this, SLOT(changePersistent()));
    connect(setup_persistent_file_, SIGNAL(changed()), this, SLOT(loadPersistedSelection()));

    updateLocation(0,0);

}

void LH_CursorController::doMoveUp(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_location_->active )
    {
        updateLocation(0,-1);
    }
}

void LH_CursorController::doMoveDown(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_location_->active )
    {
        updateLocation(0,1);
    }
}

void LH_CursorController::doMoveLeft(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_location_->active )
    {
        updateLocation(-1,0);
    }
}

void LH_CursorController::doMoveRight(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);
    if( cursor_location_->active )
    {
        updateLocation(1,0);
    }
}

void LH_CursorController::doSelect(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);

    if( cursor_location_->active )
    {
        cursor_location_->sendSelect = false;
        if(cursorModes[setup_mode_->value()].select==smSelectDeselect &&
           cursor_location_->selState &&
           cursor_location_->selX == cursor_location_->x &&
           cursor_location_->selY == cursor_location_->y)
        {
            cursor_location_->selState = false;
        }
        else
        {
            cursor_location_->selX = cursor_location_->x;
            cursor_location_->selY = cursor_location_->y;
            cursor_location_->selState = true;

            if(cursor_location_->lastSelSet)
            {
                cursor_location_->lastSelX2 = cursor_location_->lastSelX;
                cursor_location_->lastSelY2 = cursor_location_->lastSelY;
            } else {
                cursor_location_->lastSelX2 = cursor_location_->selX;
                cursor_location_->lastSelY2 = cursor_location_->selY;
            }

            cursor_location_->lastSelSet = true;
            cursor_location_->lastSelX = cursor_location_->selX;
            cursor_location_->lastSelY = cursor_location_->selY;
        }
        persistSelection();
    }
}

void LH_CursorController::doReselect(QString key,int flags,int value)
{
    if( cursor_location_->active )
    {
        updateLocation(cursor_location_->lastSelX, cursor_location_->lastSelY, true);
        doSelect(key,flags,value);
    }
}

void LH_CursorController::persistSelection()
{
    if (!setup_persistent_->value()) return;
    if (setup_persistent_file_->value().isFile())
    {
        QString selString = QString("%1,%2,%3,%4,%5").arg(cursor_location_->x).arg(cursor_location_->y).arg(cursor_location_->selState).arg(cursor_location_->selX).arg(cursor_location_->selY);
        QFile persistFile ( setup_persistent_file_->value().absoluteFilePath() );
        persistFile.open(QIODevice::WriteOnly|QFile::Truncate);
        persistFile.write(selString.toUtf8());
        persistFile.close();
    }
}

void LH_CursorController::loadPersistedSelection()
{
    if (!setup_persistent_->value()) return;
    if (setup_persistent_file_->value().isFile())
    {
        QFile persistFile ( setup_persistent_file_->value().absoluteFilePath() );
        persistFile.open(QIODevice::ReadOnly);
        QString selString = QString(persistFile.readAll());
        persistFile.close();

        QStringList selData = selString.split(',');
        if (selData.count()>=5)
        {
            updateLocation(selData[0].toInt(), selData[1].toInt(), true);

            cursor_location_->lastSelSet = true;
            cursor_location_->lastSelX = cursor_location_->x;
            cursor_location_->lastSelY = cursor_location_->y;
            cursor_location_->lastSelX2 = cursor_location_->x;
            cursor_location_->lastSelY2 = cursor_location_->y;

            if(setup_persistent_autoselect_->value())
            {
                cursor_location_->selState = (selData[2] == "1");
                cursor_location_->selX = selData[3].toInt();
                cursor_location_->selY = selData[4].toInt();
            }
        }
    }
}

void LH_CursorController::doActivate(QString key,int flags,int value)
{
    Q_UNUSED(key);
    Q_UNUSED(flags);
    Q_UNUSED(value);

    if( cursorModes[setup_mode_->value()].activate )
    {
        cursor_location_->active = !cursor_location_->active;
        updateLocation(0,0);
    }
}

void LH_CursorController::updateLocation(int xMod, int yMod, bool absolute)
{
    QStringList boundsList = setup_boundry_->value().split(',');
    QStringList badPoints = setup_invalid_coordinates_->value().split(';');

    bool moved = false;

    int newX = cursor_location_->x;
    int newY = cursor_location_->y;

    if (boundsList.length()>=4)
    {
        cursor_location_->range.x = (minmax){boundsList.at(0).toInt(), boundsList.at(2).toInt()};
        cursor_location_->range.y = (minmax){boundsList.at(1).toInt(), boundsList.at(3).toInt()};

        if (absolute)
        {
            if(xMod >= cursor_location_->range.x.min &&
               xMod <= cursor_location_->range.x.max &&
               yMod >= cursor_location_->range.y.min &&
               yMod <= cursor_location_->range.y.max
               )
            {
                newX = xMod;
                newY = yMod;
                QString point = QString::number(newX) + "," + QString::number(newY);
                if(!badPoints.contains(point)) moved = true;
            }
        } else
        if(xMod!=0 || yMod!=0)
        {
            int loops = 0;
            while (loops<=1 && !moved)
            {
                loops++;
                while(!moved &&
                      newX+xMod >= cursor_location_->range.x.min &&
                      newX+xMod <= cursor_location_->range.x.max &&
                      newY+yMod >= cursor_location_->range.y.min &&
                      newY+yMod <= cursor_location_->range.y.max
                      )
                {
                    newX += xMod;
                    newY += yMod;
                    QString point = QString::number(newX) + "," + QString::number(newY);
                    if(!badPoints.contains(point)) moved = true;
                    if(!setup_jump_invalid_->value()) break;


                }
                if(!setup_boundry_loop_->value()) break;
                else if(!moved)
                {
                    if(xMod==1)  newX = cursor_location_->range.x.min - xMod;
                    if(xMod==-1) newX = cursor_location_->range.x.max - xMod;
                    if(yMod==1)  newY = cursor_location_->range.y.min - yMod;
                    if(yMod==-1) newY = cursor_location_->range.y.max - yMod;
                }
            }
        }
    }

    if(moved) {
        cursor_location_->x = newX;
        cursor_location_->y = newY;
        if(cursorModes[setup_mode_->value()].select==smNone) doSelect("",0,0);
    }

    persistSelection();

    setText(QString::number(cursor_location_->x) + "," + QString::number(cursor_location_->y));
    setup_fontresize_->setFlag(LH_FLAG_HIDDEN,true);
    setup_pencolor_->setFlag(LH_FLAG_HIDDEN,true);

    callback(lh_cb_render,NULL);
}

void LH_CursorController::changeMode()
{
    if(cursorModes[setup_mode_->value()].select!=smSelectDeselect)
    {
        cursor_location_->selX = cursor_location_->x;
        cursor_location_->selY = cursor_location_->y;
        cursor_location_->selState = true;
        //qDebug() << "Selected";
    }
    cursor_location_->active = !cursorModes[setup_mode_->value()].activate;
    setup_select_->setFlag(LH_FLAG_HIDDEN, cursorModes[setup_mode_->value()].select==smNone);
    setup_reselect_->setFlag(LH_FLAG_HIDDEN, cursorModes[setup_mode_->value()].select!=smSelectDeselect);
    setup_activate_->setFlag(LH_FLAG_HIDDEN, !cursorModes[setup_mode_->value()].activate);
}

void LH_CursorController::changeBounds()
{
    QStringList bounds = setup_boundry_->value().split(',');
    bool showHorz = false;
    bool showVert = false;
    if(bounds.length()>=4)
    {
        showHorz = bounds.at(0) != bounds.at(2);
        showVert = bounds.at(1) != bounds.at(3);

        setup_move_up_->setFlag(   LH_FLAG_HIDDEN, !showVert);
        setup_move_down_->setFlag( LH_FLAG_HIDDEN, !showVert);
        setup_move_left_->setFlag( LH_FLAG_HIDDEN, !showHorz);
        setup_move_right_->setFlag(LH_FLAG_HIDDEN, !showHorz);
    }
}

#if 0
LH_CursorController::~LH_CursorController()
{
    // closeMemory();
}
#endif

int LH_CursorController::polling()
{
    if(cursor_location_->sendSelect) doSelect(0,0,0);
    return 200;
}

#if 0
bool LH_CursorController::openMemory()
{
    const char* mapname  = "LHCursorSharedMemory";

    if( shmem_ )

    if(debugMemory) qDebug() << "LH_CursorController: open memory: CreateFileMapping";
    memMap = (HANDLE)CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(cursorData),mapname); //INVALID_HANDLE_VALUE

    if(memMap)
    {
        if(debugMemory) qDebug() << "LH_CursorController: open memory: MapViewOfFile";
        cursor_location_ = (cursorData*)MapViewOfFile(memMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, sizeof(cursorData));
        if(cursor_location_)
        {
            if(debugMemory) qDebug() << "LH_CursorController: open memory: memset";
            memset(cursor_location_, '\0', sizeof(cursorData));
            if(debugMemory) qDebug() << "LH_CursorController: open memory: done";

            cursor_location_->active = false;
            cursor_location_->x=1;
            cursor_location_->y=1;
            cursor_location_->selX = 0;
            cursor_location_->selY = 0;
            cursor_location_->selState = false;

            return true;
        } else
            if(debugMemory) qDebug() << "LH_CursorController: open memory: failed";
    } else
        if(debugMemory) qDebug() << "LH_CursorController: open memory: failed";
    return false;
}
void LH_CursorController::closeMemory()
{
    if(cursor_location_)
    {
        if(debugMemory) qDebug() << "LH_CursorController: close memory: UnmapViewOfFile";
        UnmapViewOfFile(cursor_location_);
    }
    if(memMap)
    {
        if(debugMemory) qDebug() << "LH_CursorController: close memory: CloseHandle (memMap)";
        CloseHandle(memMap);
    }
    if(debugMemory) qDebug() << "LH_CursorController: close memory: Done.";
}
#endif
void LH_CursorController::changePersistent()
{
    setup_persistent_file_->setFlag(LH_FLAG_HIDDEN, !setup_persistent_->value());
    setup_persistent_autoselect_->setFlag(LH_FLAG_HIDDEN, !setup_persistent_->value());
}
