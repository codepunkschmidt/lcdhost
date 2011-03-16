/**
  \file     LH_CursorAction.cpp
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

#include "LH_CursorAction.h"
#include <QProcess>
#include <QDesktopServices>

LH_PLUGIN_CLASS(LH_CursorAction)

lh_class *LH_CursorAction::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorAction",
        "Cursor Action",
        48,48,
        lh_instance_calltable_NULL
    };
    return &classInfo;
}

LH_CursorAction::LH_CursorAction(const char *name, LH_QtPlugin *parent ) : LH_QtInstance( name, 0, parent )
{
    waiting = false;
    selected = false;
    fired = false;
    delay = 0;
    setup_coordinate_ = new LH_Qt_QString(this, "Coordinate", "1,1", LH_FLAG_AUTORENDER);
    setup_coordinate_->setHelp("This is the coordinate of this object, i.e. when the cursor is at the point specified here this object is selected. <br/>"
                               "<br/>"
                               "Note that many objects can have the same coordinate if the user requires.<br/>"
                               "<br/>"
                               "The format is [x],[y] <br/>"
                               "e.g.: 1,1<br/>"
                               "<br/>"
                               "Note that cursor pages are not easy to work with and the guide should be consulted (see the Plugins tab for more information.<br/>"
                               "<br/>"
                               "In short the cursor page works by changing its width (which only works if the width is set to \"adjust\") which will then make any objects that are aligned to the right edge of the page move off & on screen creating the illusion of a page of objects being hidden/shown."
                               );

    setup_jump_to_ = new LH_Qt_InputState(this, "Quick Select", "");
    setup_jump_to_->setHelp("This optional field allows you to bind a specific key to this coordinate; when that key is pressed the cursor immediately jumps to and selects the coordintes.<br/>"
                               "<br/>"
                               "Note that this functionality is meant more for pages than for menus, but can be used on either."
                               );
    connect( setup_jump_to_, SIGNAL(input(QString,int,int)), this, SLOT(doJumpTo(QString,int,int)) );

    statusCode_ = "OFF";

    setup_actions_ = new LH_Qt_QStringList(this, "Actions", QStringList(),LH_FLAG_NOSAVE,lh_type_integer_listbox);
    setup_actions_->setHelp("This box contains the list of actions that will be perfromed when the coordinate is selected. Each action is done in the order shown here.<br/><br/>Select an action from this list to edit or delete it.");
    setup_action_add_ = new LH_Qt_QString(this, "^AddNewAction","Add New Action", LH_FLAG_NOSAVE, lh_type_string_button);

    setup_action_desc_ = new LH_Qt_QString(this, "Action Description", "",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_index_ = new LH_Qt_int(this, "Action Index",0, LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_type_ = new LH_Qt_QStringList(this, "Action Type", actionTypes_.list(),LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);

    setup_action_parameter1_desc_ = new LH_Qt_QString(  this, "^p0_desc","",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_parameter1_str_  = new LH_Qt_QString(  this, "^p0_str" ,"",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_parameter1_int_  = new LH_Qt_int(      this, "^p0_int" ,0,LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_parameter1_file_ = new LH_Qt_QFileInfo(this, "^p0_file",QFileInfo(),LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);

    setup_action_parameter2_desc_ = new LH_Qt_QString(  this, "^p1_desc","",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_parameter2_str_  = new LH_Qt_QString(  this, "^p1_str" ,"",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_parameter2_int_  = new LH_Qt_int(      this, "^p1_int" ,0,LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    setup_action_parameter2_file_ = new LH_Qt_QFileInfo(this, "^p1_file",QFileInfo(),LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);

    setup_action_enabled_= new LH_Qt_bool(this, "^Action Enabled", true, LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);

    setup_action_delete_ = new LH_Qt_QString(this, "^DeleteAction","Delete Action", LH_FLAG_HIDDEN|LH_FLAG_NOSAVE, lh_type_string_button);
    setup_action_delete_confirm_ = new LH_Qt_QString(this, "^DeleteActionConfirm", "Confirm Delete", LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_NOSAVE, lh_type_string_button);


    setup_actions_xml_ = new LH_Qt_QTextEdit(this, tr("~"), "<actions>\n</actions>", LH_FLAG_HIDDEN);

    connect( setup_actions_xml_,            SIGNAL(changed()), this, SLOT(xmlChanged()));
    connect( setup_actions_,                SIGNAL(changed()), this, SLOT(actionSelected()));

    connect( setup_action_desc_,            SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_index_,           SIGNAL(changed()), this, SLOT(actionMoved()));
    connect( setup_action_type_,            SIGNAL(changed()), this, SLOT(actionEdited()));

    connect( setup_action_parameter1_str_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter1_int_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter1_file_, SIGNAL(changed()), this, SLOT(actionEdited()));

    connect( setup_action_parameter2_str_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter2_int_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter2_file_, SIGNAL(changed()), this, SLOT(actionEdited()));

    connect( setup_action_enabled_,         SIGNAL(changed()), this, SLOT(actionEdited()));

    connect( setup_action_delete_,          SIGNAL(changed()), this, SLOT(deleteActionCheck()));
    connect( setup_action_delete_confirm_,  SIGNAL(changed()), this, SLOT(deleteAction()));
    connect( setup_action_add_,             SIGNAL(changed()), this, SLOT(newAction()));
}

int LH_CursorAction::polling()
{
    if(waiting)
    {
        delay-=200;
        if (delay<=0) fire(lastStep+1);
    } else
        if(updateState())  callback(lh_cb_render,NULL);

    return 200;
}

cursorData LH_CursorAction::getCursorData()
{
    cursorData resultData;

    const char* mapname  = "LHCursorSharedMemory";
    // Create file mapping
    HANDLE memMap = (HANDLE)CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(cursorData),mapname);
    if(memMap)
    {
        cursorData* cursor_location_ = (cursorData*)MapViewOfFile(memMap, FILE_MAP_READ, 0, 0, sizeof(cursorData));

        if (cursor_location_) {
            resultData = *cursor_location_;
            UnmapViewOfFile(cursor_location_);
        }

        CloseHandle(memMap);
    }

    return resultData;
}

void LH_CursorAction::setCursorData( cursorData cd )
{
    const char* mapname  = "LHCursorSharedMemory";
    // Create file mapping
    HANDLE memMap = (HANDLE)CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(cursorData),mapname);
    if(memMap)
    {
        cursorData* cursor_location_ = (cursorData*)MapViewOfFile(memMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, sizeof(cursorData));

        if (cursor_location_) {
            cursor_location_->active = cd.active;
            cursor_location_->lastSelSet = cd.lastSelSet;
            cursor_location_->lastSelX = cd.lastSelX;
            cursor_location_->lastSelY = cd.lastSelY;
            cursor_location_->selState = cd.selState;
            cursor_location_->selX = cd.selX;
            cursor_location_->selY = cd.selY;
            cursor_location_->x = cd.x;
            cursor_location_->y = cd.y;
            cursor_location_->sendSelect = cd.sendSelect;
            cursor_location_->lastSelX2 = cd.lastSelX2;
            cursor_location_->lastSelY2 = cd.lastSelY2;
            cursor_location_->range = cd.range;
            UnmapViewOfFile(cursor_location_);
        }

        CloseHandle(memMap);
    }
}


bool LH_CursorAction::updateState()
{
    cursorData cd = getCursorData();
    QStringList mycoords = setup_coordinate_->value().split(';');

    bool newSelected = false;
    bool newActive = false;
    foreach (QString mycoord_str, mycoords)
    {
        QStringList mycoord = mycoord_str.split(',');
        if(mycoord.length()==2)
        {
            int myX = mycoord.at(0).toInt();
            int myY = mycoord.at(1).toInt();

            newSelected = newSelected || ( cd.selState && cd.selX==myX && cd.selY==myY );
            newActive = newActive ||  ( cd.active && cd.x==myX && cd.y==myY );
        }
    }
    QString newStatusCode = QString("%1%2").arg(newActive? "ON" : "OFF").arg(newSelected? "_SEL" : "");
    if(statusCode_ != newStatusCode)
    {
        statusCode_ = newStatusCode;
        if(selected!=newSelected)
        {
            selected = newSelected;
            if(!newSelected) fired = false; else
            if(!fired) fire();
        }
        return true;
    }
    else
        return false;
}

void LH_CursorAction::fire(int startAt)
{
    fired = true;
    waiting = false;
    QDomDocument actionsXML("actionsXML");
    if(actionsXML.setContent(setup_actions_xml_->value()))
    {
        QDomNode rootNode = actionsXML.firstChild();
        for(uint i = startAt; i< rootNode.childNodes().length(); i++)
        {
            lastStep = i;
            QDomNode n = rootNode.childNodes().at(i);
            QDomElement e = n.toElement();
            if(e.attribute("enabled")=="false")
                continue;
            QString typeCode = e.attribute("type");
            actionType action = actionTypes_.at(typeCode);
            if(typeCode=="open")
            {
                QString layout = action.getParameter(e,0);
                if (!layout.contains(":"))
                    layout = QString("%1\%2").arg(lcdhost_state()->dir_layout).arg(layout);
                static QByteArray ary;
                ary = layout.toUtf8();
                callback(lh_cb_load_layout, ary.data() );
            }else
            if(typeCode=="run")
            {
                QProcess process;
                QString path = action.getParameter(e,0);
                QStringList args = QStringList() << action.getParameter(e,1);
                process.startDetached(path,args,lcdhost_state()->dir_layout);
            }else
            if(typeCode=="url")
            {
                QDesktopServices::openUrl( action.getParameter(e,0) );
            }else
            if(typeCode=="wait")
            {
                waiting = true;
                delay = action.getParameter(e,0).toInt();
                break;
            }else
            if(typeCode=="move")
            {
                cursorData cd = getCursorData();
                cd.x = action.getParameter(e,0).toInt();
                cd.y = action.getParameter(e,1).toInt();
                setCursorData(cd);
            }else
            if(typeCode=="select")
            {
                cursorData cd = getCursorData();
                cd.x = action.getParameter(e,0).toInt();
                cd.y = action.getParameter(e,1).toInt();
                cd.sendSelect = true;
                setCursorData(cd);
            }else
            if(typeCode=="deselect")
            {
                cursorData cd = getCursorData();
                cd.selState = false;
                setCursorData(cd);
            }else
            if(typeCode=="deactivate")
            {
                cursorData cd = getCursorData();
                cd.active = false;
                setCursorData(cd);
            }else
            if(typeCode=="reselect")
            {
                cursorData cd = getCursorData();
                cd.x = cd.lastSelX2;
                cd.y = cd.lastSelY2;
                cd.sendSelect = true;
                setCursorData(cd);
            } else
                Q_ASSERT(false);
        }
    }
}

void LH_CursorAction::doJumpTo(QString key, int flags, int value)
{
    QString coord = setup_coordinate_->value().split(';')[0];
    cursorData cd = getCursorData();
    cd.x = coord.split(',')[0].toInt();
    cd.y = coord.split(',')[1].toInt();
    cd.sendSelect = true;
    setCursorData(cd);
}

void LH_CursorAction::xmlChanged()
{
    setup_actions_->list().clear();
    QDomDocument actionsXML("actionsXML");
    if(actionsXML.setContent(setup_actions_xml_->value()))
    {
        QDomNode rootNode = actionsXML.firstChild();
        for(uint i = 0; i< rootNode.childNodes().length(); i++)
        {
            QDomNode n = rootNode.childNodes().at(i);
            QDomElement e = n.toElement();

            QString desc = (e.attribute("desc")==""? actionTypes_.at(e.attribute("type")).description : e.attribute("desc"));
            if(e.attribute("enabled")=="false") desc+=" [disabled]";

            setup_actions_->list().append(desc);
        }
    }
    setup_actions_->refreshList();
    actionSelected();
}

void LH_CursorAction::actionSelected()
{
    int offFlag = 0;
    if(setup_actions_->list().count()==0) offFlag = LH_FLAG_HIDDEN | LH_FLAG_READONLY;

    setup_action_desc_->setFlags(LH_FLAG_NOSAVE | offFlag);
    setup_action_index_->setFlags(LH_FLAG_NOSAVE | offFlag);
    setup_action_index_->setMinMax(0,setup_actions_->list().count()-1);

    setup_action_type_->setFlags(LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter1_desc_->setFlags(LH_FLAG_READONLY|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter1_str_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter1_int_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter1_file_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter2_desc_->setFlags(LH_FLAG_READONLY|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter2_str_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter2_int_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_NOSAVE | offFlag);
    setup_action_parameter2_file_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_NOSAVE | offFlag);
    setup_action_delete_->setFlags(LH_FLAG_NOSAVE | offFlag);
    setup_action_delete_confirm_->setFlags(LH_FLAG_HIDDEN|LH_FLAG_READONLY|LH_FLAG_NOSAVE | offFlag);
    setup_action_enabled_->setFlags(LH_FLAG_NOSAVE | offFlag);

    if(setup_actions_->list().count()==0) return;

    QDomDocument actionsXML("actionsXML");
    if(actionsXML.setContent(setup_actions_xml_->value()) && actionsXML.firstChild().childNodes().count()!=0 && setup_actions_->value()!=-1)
    {
        QDomElement e = actionsXML.firstChild().childNodes().at(setup_actions_->value()).toElement();
        QString typeCode = e.attribute("type");

        setup_action_type_->setValue( actionTypes_.indexOf(typeCode) );
        setup_action_desc_->setValue( e.attribute("desc") );
        setup_action_index_->setValue( setup_actions_->value() );

        actionTypes_.at(typeCode).displayParameter(0,setup_action_parameter1_desc_,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_, getCursorData(), e);
        actionTypes_.at(typeCode).displayParameter(1,setup_action_parameter2_desc_,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_, getCursorData(), e);

        setup_action_enabled_->setValue( e.attribute("enabled")!="false" );
    }
}

void LH_CursorAction::actionEdited()
{
    if(setup_actions_->list().count()==0) return;
    actionType at = actionTypes_.at(setup_action_type_->value());

    //acquire valid parameter values (blank values return in parameter is invalid)
    QStringList paramValues = QStringList();
    paramValues.append( at.getParameterValue(0,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_));
    paramValues.append( at.getParameterValue(1,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_));

    //in case the type has changed, update the parameter visibility
    at.displayParameter(0,setup_action_parameter1_desc_,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_, getCursorData());
    at.displayParameter(1,setup_action_parameter2_desc_,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_, getCursorData());

    //Update the caption in case the description or enabled state has changed
    QString desc = (setup_action_desc_->value()==""? at.description : setup_action_desc_->value());
    if(!setup_action_enabled_->value()) desc+=" [disabled]";
    setup_actions_->list()[setup_actions_->value()] = desc;
    setup_actions_->refreshList();

    //Update the xml data
    QStringList xmlLines = setup_actions_xml_->value().split("\n");
    xmlLines[setup_actions_->value()+1] = at.generateXML(setup_action_enabled_->value(),setup_action_desc_->value(),paramValues);
    setup_actions_xml_->setValue(xmlLines.join("\n"));

    //reset the deletion stuff
    setup_action_delete_->setFlag(LH_FLAG_READONLY, false);
    setup_action_delete_confirm_->setFlag(LH_FLAG_READONLY, true);
    setup_action_delete_confirm_->setFlag(LH_FLAG_HIDDEN, true);
}

void LH_CursorAction::actionMoved()
{
    int curPos = setup_actions_->value();
    int newPos = setup_action_index_->value();

    QStringList xmlLines = setup_actions_xml_->value().split("\n");
    xmlLines.move(curPos+1,newPos+1);
    setup_actions_xml_->setValue(xmlLines.join("\n"));

    xmlChanged();
    setup_actions_->setValue(newPos);
    actionSelected();
}

void LH_CursorAction::newAction()
{
    actionType at = actionTypes_.at("deselect");
    QString strXML =  at.generateXML(false,"",QStringList());

    QStringList xmlLines = setup_actions_xml_->value().split("\n");
    xmlLines.insert(xmlLines.count()-1, strXML);
    setup_actions_xml_->setValue(xmlLines.join("\n"));

    xmlChanged();
    setup_actions_->setValue(setup_actions_->list().count()-1);
    actionSelected();
}

void LH_CursorAction::deleteActionCheck()
{
    setup_action_delete_->setFlag(LH_FLAG_READONLY, true);
    setup_action_delete_confirm_->setFlag(LH_FLAG_READONLY, false);
    setup_action_delete_confirm_->setFlag(LH_FLAG_HIDDEN, false);
}

void LH_CursorAction::deleteAction()
{
    int selIndex = setup_actions_->value();

    QStringList xmlLines = setup_actions_xml_->value().split("\n");
    xmlLines.removeAt(selIndex+1);
    setup_actions_xml_->setValue(xmlLines.join("\n"));

    xmlChanged();
    if(selIndex>=setup_actions_->list().length()) selIndex--;
    setup_actions_->setValue(selIndex);
    actionSelected();
}

