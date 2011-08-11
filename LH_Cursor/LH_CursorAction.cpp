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
#include "../LH_QtPlugin.h"
#include <QProcess>
#include <QDesktopServices>

LH_PLUGIN_CLASS(LH_CursorAction)

const int LH_FLAG_UI = LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_NOSOURCE;

lh_class *LH_CursorAction::classInfo()
{
    static lh_class classInfo =
    {
        sizeof(lh_class),
        "Cursor",
        "CursorAction",
        "Cursor Action",
        48,48
        
        
    };
    return &classInfo;
}

LH_CursorAction::LH_CursorAction() : actionTypes_(this)
{
    waiting = false;
    selected = false;
    fired = false;
    delay = 0;
    statusCode_ = "OFF";
}

const char *LH_CursorAction::userInit()
{
    if( const char *err = LH_QtInstance::userInit() ) return err;

    hide();

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

    setup_json_data_ = new LH_Qt_QString(this, "Cursor Data", "", LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_LAST /*| LH_FLAG_READONLY | LH_FLAG_HIDEVALUE*/);
    setup_json_data_->setLink("Cursors/Primary Cursor");
    setup_json_data_->setLinkFilter("Cursors");
    setup_json_data_->refreshData();

    setup_json_postback_ = new LH_Qt_QString(this, "Cursor Postback", "", LH_FLAG_NOSAVE | LH_FLAG_NOSINK | LH_FLAG_LAST | LH_FLAG_READONLY );
    setup_json_postback_->setLink("Cursors/Postback", true);
    setup_json_postback_->setLinkFilter("CursorPostback");
    setup_json_postback_->refreshData();

    setup_jump_to_ = new LH_Qt_InputState(this, "Quick Select", "", LH_FLAG_NOSINK | LH_FLAG_NOSOURCE);
    setup_jump_to_->setHelp("This optional field allows you to bind a specific key to this coordinate; when that key is pressed the cursor immediately jumps to and selects the coordintes.<br/>"
                               "<br/>"
                               "Note that this functionality is meant more for pages than for menus, but can be used on either."
                               );

    setup_act_copy_ = new LH_Qt_QString(this, "Copy Actions", "Copy",  LH_FLAG_UI | LH_FLAG_BLANKTITLE, lh_type_string_button);
    setup_act_paste_ = new LH_Qt_QString(this, "Paste Actions", "Paste",  LH_FLAG_UI | LH_FLAG_BLANKTITLE, lh_type_string_button);

    setup_act_rules_ = new LH_Qt_QStringList(this, "Actions", QStringList(),LH_FLAG_UI,lh_type_string_listbox);
    setup_act_rules_->setHelp("This box contains the list of actions that will be perfromed when the coordinate is selected. Each action is done in the order shown here.<br/><br/>Select an action from this list to edit or delete it.");

    setup_act_move_up_= new LH_Qt_QString(this, "Move Action Up", "Move Up",  LH_FLAG_UI | LH_FLAG_READONLY | LH_FLAG_BLANKTITLE, lh_type_string_button);
    setup_act_move_down_= new LH_Qt_QString(this, "Move Action Down", "Move Down",  LH_FLAG_UI | LH_FLAG_READONLY | LH_FLAG_BLANKTITLE, lh_type_string_button);
    setup_act_new_ = new LH_Qt_QString(this, "New Action", "New",  LH_FLAG_UI | LH_FLAG_BLANKTITLE, lh_type_string_button);
    setup_act_delete_ = new LH_Qt_QString(this, "Delete Action", "Delete", LH_FLAG_UI | LH_FLAG_READONLY | LH_FLAG_BLANKTITLE, lh_type_string_button);

    setup_action_desc_ = new LH_Qt_QString(this, "Description", "",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI);
    setup_action_type_ = new LH_Qt_QStringList(this, "Action Type", actionTypes_.list(),LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI);

    setup_action_parameter1_desc_ = new LH_Qt_QString(  this, "p0_desc","",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);
    setup_action_parameter1_str_  = new LH_Qt_QString(  this, "p0_str" ,"",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);
    setup_action_parameter1_int_  = new LH_Qt_int(      this, "p0_int" ,0,LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);
    setup_action_parameter1_file_ = new LH_Qt_QFileInfo(this, "p0_file",QFileInfo(),LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);

    setup_action_parameter2_desc_ = new LH_Qt_QString(  this, "p1_desc","",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);
    setup_action_parameter2_str_  = new LH_Qt_QString(  this, "p1_str" ,"",LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);
    setup_action_parameter2_int_  = new LH_Qt_int(      this, "p1_int" ,0,LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);
    setup_action_parameter2_file_ = new LH_Qt_QFileInfo(this, "p1_file",QFileInfo(),LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);

    setup_action_enabled_= new LH_Qt_bool(this, "Action Enabled", true, LH_FLAG_READONLY|LH_FLAG_HIDDEN|LH_FLAG_UI | LH_FLAG_BLANKTITLE);

    setup_act_save_   = new LH_Qt_QString(this, "Save Condition", "Save", LH_FLAG_UI | LH_FLAG_HIDDEN | LH_FLAG_BLANKTITLE, lh_type_string_button);
    setup_act_cancel_ = new LH_Qt_QString(this, "Cancel Condition Edit", "Cancel", LH_FLAG_UI | LH_FLAG_HIDDEN | LH_FLAG_BLANKTITLE, lh_type_string_button);

    setup_act_XML_ = new LH_Qt_QTextEdit(this, "Action XML", "<actions>\n</actions>", LH_FLAG_HIDDEN | LH_FLAG_HIDETITLE);

    connect(setup_json_data_,SIGNAL(changed()),this,SLOT(updateState()));

    connect( setup_jump_to_,                SIGNAL(input(int,int)), this, SLOT(doJumpTo(int,int)) );
    connect( setup_act_XML_,                SIGNAL(changed()), this, SLOT(xmlChanged()));
    connect( setup_act_rules_,              SIGNAL(changed()), this, SLOT(reloadAction()));

    connect( setup_action_type_,            SIGNAL(changed()), this, SLOT(actionTypeChanged()));

    /*connect( setup_action_desc_,            SIGNAL(changed()), this, SLOT(actionEdited()));
    //connect( setup_action_index_,           SIGNAL(changed()), this, SLOT(actionMoved()));
    connect( setup_action_enabled_,         SIGNAL(changed()), this, SLOT(actionEdited()));

    connect( setup_action_parameter1_str_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter1_int_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter1_file_, SIGNAL(changed()), this, SLOT(actionEdited()));

    connect( setup_action_parameter2_str_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter2_int_ , SIGNAL(changed()), this, SLOT(actionEdited()));
    connect( setup_action_parameter2_file_, SIGNAL(changed()), this, SLOT(actionEdited()));*/

    connect(setup_act_new_,       SIGNAL(changed()), this, SLOT(newAction()));
    connect(setup_act_save_,      SIGNAL(changed()), this, SLOT(saveAction()));
    connect(setup_act_cancel_,    SIGNAL(changed()), this, SLOT(uneditAction()));
    connect(setup_act_delete_,    SIGNAL(changed()), this, SLOT(deleteAction()));
    connect(setup_act_move_up_,   SIGNAL(changed()), this, SLOT(moveAction_up()));
    connect(setup_act_move_down_, SIGNAL(changed()), this, SLOT(moveAction_down()));
    connect(setup_act_copy_,      SIGNAL(changed()), this, SLOT(copyActions()));
    connect(setup_act_paste_,     SIGNAL(changed()), this, SLOT(pasteActions()));

    return 0;
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

bool LH_CursorAction::updateState()
{
    bool newSelected;
    bool newActive;
    QString newStatusCode = cursorData(setup_json_data_->value()).getState(setup_coordinate_->value().split(';'),newSelected,newActive);

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
    cursorData cursor_data(setup_json_data_->value());
    fired = true;
    waiting = false;
    QDomDocument actionsXML("actionsXML");
    if(actionsXML.setContent(setup_act_XML_->value()))
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
                    layout = QString("%1\%2").arg(dir_layout()).arg(layout);
                static QByteArray ary;
                ary = layout.toUtf8();
                callback(lh_cb_load_layout, ary.data() );
            }else
            if(typeCode=="run")
            {
                QRegExp rx("((?:[^\\s\"]*(?:\"[^\"]*\")?)*)");
                QProcess process;
                QString path = action.getParameter(e,0);
                QFileInfo exe = QFileInfo(path);
                if(!exe.isFile())
                    exe = QFileInfo(QString("%1%2").arg(dir_layout()).arg(path));
                QString argsString = action.getParameter(e,1);
                QStringList argsList;
                if(rx.indexIn(argsString) != -1)
                    for(int i=1; i<=rx.captureCount(); i++)
                        if(rx.cap(i)!="")
                            argsList.append(rx.cap(i));
                process.startDetached(exe.absoluteFilePath(),argsList,dir_layout());
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
                cursor_data.x = action.getParameter(e,0).toInt();
                cursor_data.y = action.getParameter(e,1).toInt();
                cursor_data.postback(setup_json_postback_, setup_json_data_);
            }else
            if(typeCode=="select")
            {
                cursor_data.x = action.getParameter(e,0).toInt();
                cursor_data.y = action.getParameter(e,1).toInt();
                cursor_data.sendSelect = true;
                cursor_data.postback(setup_json_postback_, setup_json_data_);
            }else
            if(typeCode=="deselect")
            {
                cursor_data.selState = false;
                cursor_data.postback(setup_json_postback_, setup_json_data_);
            }else
            if(typeCode=="deactivate")
            {
                cursor_data.active = false;
                cursor_data.postback(setup_json_postback_, setup_json_data_);
            }else
            if(typeCode=="reselect")
            {
                cursor_data.x = cursor_data.lastSelX2;
                cursor_data.y = cursor_data.lastSelY2;
                cursor_data.sendSelect = true;
                cursor_data.postback(setup_json_postback_, setup_json_data_);
            } else
                qWarning() << "LH_Cursor: Unknown Action: " << typeCode;
        }
    }
}

void LH_CursorAction::doJumpTo(int flags, int value)
{
    Q_UNUSED(flags);
    Q_UNUSED(value);
    QString coord = setup_coordinate_->value().split(';')[0];
    cursorData cursor_data(setup_json_data_->value());
    cursor_data.x = coord.split(',')[0].toInt();
    cursor_data.y = coord.split(',')[1].toInt();
    cursor_data.sendSelect = true;
    cursor_data.postback(setup_json_postback_, setup_json_data_);
}

void LH_CursorAction::xmlChanged()
{
    setup_act_rules_->list().clear();
    QDomDocument actionsXML("actionsXML");
    if(actionsXML.setContent(setup_act_XML_->value()))
    {
        QDomNode rootNode = actionsXML.firstChild();
        for(uint i = 0; i< rootNode.childNodes().length(); i++)
        {
            QDomNode n = rootNode.childNodes().at(i);
            QDomElement e = n.toElement();

            QString desc = (e.attribute("desc")==""? actionTypes_.at(e.attribute("type")).description : e.attribute("desc"));
            if(e.attribute("enabled")=="false") desc+=" [disabled]";

            setup_act_rules_->list().append(desc);
        }
    }
    setup_act_rules_->refreshList();
    reloadAction();
}

void LH_CursorAction::enableEditUI(bool enabled)
{
    int offFlag = (enabled? 0 : LH_FLAG_HIDDEN | LH_FLAG_READONLY);
    int curPos = setup_act_rules_->index();

    setup_action_desc_->setFlags(LH_FLAG_UI | offFlag);
    setup_action_type_->setFlags(LH_FLAG_UI | offFlag);

    setup_action_parameter1_desc_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_UI | offFlag);
    setup_action_parameter1_str_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_UI | offFlag);
    setup_action_parameter1_int_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_UI | offFlag);
    setup_action_parameter1_file_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_UI | offFlag);

    setup_action_parameter2_desc_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_READONLY | LH_FLAG_UI | offFlag);
    setup_action_parameter2_str_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_UI | offFlag);
    setup_action_parameter2_int_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_UI | offFlag);
    setup_action_parameter2_file_->setFlags(LH_FLAG_HIDDEN | LH_FLAG_UI | offFlag);

    setup_action_enabled_->setFlags(LH_FLAG_UI | offFlag);

    setup_act_save_->setFlags(LH_FLAG_UI | offFlag);
    setup_act_move_up_->setFlags(LH_FLAG_UI | (offFlag & LH_FLAG_READONLY) | (curPos <= 0 ? LH_FLAG_READONLY : 0) );
    setup_act_move_down_->setFlags(LH_FLAG_UI | (offFlag & LH_FLAG_READONLY) | ( curPos == -1 || curPos >= setup_act_rules_->list().count()-1? LH_FLAG_READONLY : 0) );
    setup_act_delete_->setFlags(LH_FLAG_UI | (offFlag & LH_FLAG_READONLY) | (curPos == -1 ? LH_FLAG_READONLY : 0) );
    setup_act_cancel_->setFlags(LH_FLAG_UI | offFlag);
}

void LH_CursorAction::reloadAction()
{
    enableEditUI(setup_act_rules_->list().count()!=0 && setup_act_rules_->index()!=-1);
    if(setup_act_rules_->list().count()==0) return;

    QDomDocument actionsXML("actionsXML");
    if(actionsXML.setContent(setup_act_XML_->value()) && actionsXML.firstChild().childNodes().count()!=0 && setup_act_rules_->index()!=-1)
    {
        QDomElement e = actionsXML.firstChild().childNodes().at(setup_act_rules_->index()).toElement();
        QString typeCode = e.attribute("type");

        setup_action_type_->setValue( actionTypes_.indexOf(typeCode) );
        setup_action_desc_->setValue( e.attribute("desc") );

        cursorData cursor_data(setup_json_data_->value());
        actionTypes_.at(typeCode).displayParameter(0,setup_action_parameter1_desc_,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_, cursor_data, e);
        actionTypes_.at(typeCode).displayParameter(1,setup_action_parameter2_desc_,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_, cursor_data, e);

        setup_action_enabled_->setValue( e.attribute("enabled")!="false" );
    }
}

void LH_CursorAction::saveAction()
{
    actionType at = actionTypes_.at(setup_action_type_->value());

    //acquire valid parameter values (blank values return in parameter is invalid)
    QStringList paramValues = QStringList();
    paramValues.append( at.getParameterValue(0,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_));
    paramValues.append( at.getParameterValue(1,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_));

    //in case the type has changed, update the parameter visibility
    cursorData cursor_data(setup_json_data_->value());
    at.displayParameter(0,setup_action_parameter1_desc_,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_, cursor_data);
    at.displayParameter(1,setup_action_parameter2_desc_,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_, cursor_data);

    //Update the caption in case the description or enabled state has changed
    QString desc = (setup_action_desc_->value()==""? at.description : setup_action_desc_->value());
    if(!setup_action_enabled_->value()) desc+=" [disabled]";

    //Update the xml data
    QString strXML = at.generateXML(setup_action_enabled_->value(),setup_action_desc_->value(),paramValues);
    QStringList xmlLines = setup_act_XML_->value().split("\n");
    if(setup_act_rules_->index()!=-1)
    {
        //setup_act_rules_->list()[setup_act_rules_->value()] = desc;
        //setup_act_rules_->refreshList();
        xmlLines[setup_act_rules_->index()+1] = strXML;
    } else {
        xmlLines.insert(xmlLines.count()-1, strXML);
        //setup_act_rules_->setValue(setup_act_rules_->list().count()-1);
    }
    setup_act_XML_->setValue(xmlLines.join("\n"));
    xmlChanged();
}

void LH_CursorAction::moveAction_up()
{
    int curPos = setup_act_rules_->index();

    if(curPos <= 0) return;

    int newPos = curPos-1;//setup_action_index_->value();

    QStringList xmlLines = setup_act_XML_->value().split("\n");
    xmlLines.move(curPos+1,newPos+1);
    setup_act_XML_->setValue(xmlLines.join("\n"));

    xmlChanged();
    setup_act_rules_->setValue(newPos);
    reloadAction();
}

void LH_CursorAction::moveAction_down()
{
    int curPos = setup_act_rules_->index();

    if(curPos >= setup_act_rules_->list().count()-1) return;

    int newPos = curPos+1;//setup_action_index_->value();

    QStringList xmlLines = setup_act_XML_->value().split("\n");
    xmlLines.move(curPos+1,newPos+1);
    setup_act_XML_->setValue(xmlLines.join("\n"));

    xmlChanged();
    setup_act_rules_->setValue(newPos);
    reloadAction();
}

void LH_CursorAction::newAction()
{
    setup_act_rules_->setValue(-1);
    enableEditUI(true);

    QString typeCode = "deselect";

    setup_action_type_->setValue( actionTypes_.indexOf(typeCode) );
    setup_action_desc_->setValue( "" );

    //actionTypes_.at(typeCode).displayParameter(0,setup_action_parameter1_desc_,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_, cursor_data, e);
    //actionTypes_.at(typeCode).displayParameter(1,setup_action_parameter2_desc_,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_, cursor_data, e);

    setup_action_enabled_->setValue( true );
}

void LH_CursorAction::deleteAction()
{
    int selIndex = setup_act_rules_->index();

    QStringList xmlLines = setup_act_XML_->value().split("\n");
    xmlLines.removeAt(selIndex+1);
    setup_act_XML_->setValue(xmlLines.join("\n"));

    xmlChanged();
    if(selIndex>=setup_act_rules_->list().length()) selIndex--;
    setup_act_rules_->setValue(selIndex);
    reloadAction();
}

void LH_CursorAction::copyActions()
{
    QFile file(QString("%1action_cache.xml").arg(dir_data()));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    else
        file.write(setup_act_XML_->value().toAscii());
}

void LH_CursorAction::pasteActions()
{
    QString clip_text;
    QFile file(QString("%1action_cache.xml").arg(dir_data()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    else
        clip_text = QString(file.readAll().data());

    QDomDocument doc_clip("");
    if(doc_clip.setContent(clip_text))
    {
        QDomElement root_clip = doc_clip.firstChild().toElement();

        QDomDocument doc_curr("");
        doc_curr.setContent(setup_act_XML_->value());
        QDomElement root_curr = doc_curr.firstChild().toElement();

        QDomNodeList actions = root_clip.elementsByTagName("action");
        while(actions.length()!=0)
            root_curr.appendChild(actions.at(0));

        setup_act_XML_->setValue(doc_curr.toString());
        xmlChanged();
    }
}

void LH_CursorAction::uneditAction(){
    setup_act_rules_->setValue(-1);
    reloadAction();
}

void LH_CursorAction::actionTypeChanged()
{
    QString typeCode = actionTypes_.at(setup_action_type_->value()).typeCode;
    cursorData cursor_data(setup_json_data_->value());
    actionTypes_.at(typeCode).displayParameter(0,setup_action_parameter1_desc_,setup_action_parameter1_str_,setup_action_parameter1_int_,setup_action_parameter1_file_, cursor_data);
    actionTypes_.at(typeCode).displayParameter(1,setup_action_parameter2_desc_,setup_action_parameter2_str_,setup_action_parameter2_int_,setup_action_parameter2_file_, cursor_data);
}
