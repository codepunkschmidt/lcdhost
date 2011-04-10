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
#include "LH_CursorInstance.h"

// #include <LH_CursorStructs.h>

#include <QHash>
#include <QList>
#include <QtXml>

enum actionParameterType
{
    aptString,
    aptFile,
    aptInteger
};

enum actionLimitType
{
    altWait,
    altCursorX,
    altCursorY
};

class actionParameter
{
public:
    QString description;
    actionParameterType type;
    QString xmlTag;
    QString xmlAttribute;
    actionLimitType limitType;

    actionParameter( QString d,
                    actionParameterType t = aptString,
                    QString xt = QString(),
                    QString xa = QString(),
                    actionLimitType lt = altWait ) :
        description(d), type(t), xmlTag(xt), xmlAttribute(xa), limitType(lt) {}
};

class actionType
{
private:
    void setIntMinMax(LH_Qt_int *int_, int minVal, int maxVal)
    {
        int val = int_->value();
        int_->setMinMax(minVal,maxVal);
        if(val<minVal)val = minVal;
        if(val>maxVal)val = maxVal;
        int_->setValue(val);
    }

public:
    QString typeCode;
    QString description;
    QList<actionParameter> parameters;

    actionType(QString typeCode__ = QString(), QString description__ = QString(), QList<actionParameter> parameters__ = QList<actionParameter>() )
        : typeCode(typeCode__), description(description__), parameters(parameters__) {}

    QString getParameter(QDomElement e, int index)
    {
        if(index<parameters.count())
        {
            QDomElement e2;
            if(parameters[index].xmlTag!="")
                e2 = e.elementsByTagName(parameters[index].xmlTag).at(0).toElement();
            else
                e2 = e;

            if(parameters[index].xmlAttribute!="")
                return e2.attribute(parameters[index].xmlAttribute);
            else
                return e2.text();
        }
        else
            return "";
    }

    QString generateXML(bool enabled, QString desc, QStringList paramValues)
    {
        QHash<QString,QString> nodesXML = QHash<QString,QString>();
        QHash<QString,QString> nodesVal = QHash<QString,QString>();
        nodesXML.insert("","");
        nodesVal.insert("","");
        nodesXML[""] += "<action ";
        nodesXML[""] += "type='" + typeCode + "' ";
        if(desc != "")
            nodesXML[""] += "desc='" + desc + "' ";
        if(!enabled)
            nodesXML[""] += "enabled='false' ";

        for(int i=0; i<parameters.length(); i++)
        {
            if(!nodesXML.contains(parameters[i].xmlTag))
            {
                nodesXML.insert(parameters[i].xmlTag,"<"+parameters[i].xmlTag+" ");
                nodesVal.insert(parameters[i].xmlTag,"");
            }
            if(parameters[i].xmlAttribute=="")
                nodesVal[parameters[i].xmlTag]+=paramValues[i];
            else
                nodesXML[parameters[i].xmlTag]+=parameters[i].xmlAttribute + "='" + paramValues[i] + "' ";
        }
        QString strXML = nodesXML[""];
        if(nodesVal[""]!="")
            strXML += ">" + nodesVal[""] + "</action>";
        else
        if(nodesXML.count()==1)
            strXML += "/>";
        else
        {
            strXML += ">";
            foreach(QString key, nodesXML.keys())
            {
                if(key=="") continue;
                strXML += nodesXML[key];
                if(nodesVal[key]!="")
                    strXML += ">" + nodesVal[key] + "</"+key+">";
                else
                    strXML += "/>";
            }
            strXML += "</action>";
        }

        return strXML;
    }

    void displayParameter(int id, LH_Qt_QString *desc_, LH_Qt_QString *str_, LH_Qt_int *int_, LH_Qt_QFileInfo *file_, cursorData cd, QDomElement e = QDomElement())
    {
        desc_->setFlag(LH_FLAG_HIDDEN, (parameters.count()<id+1));
        str_->setFlag(LH_FLAG_HIDDEN,  (parameters.count()<id+1) || (parameters[id].type != aptString));
        int_->setFlag(LH_FLAG_HIDDEN,  (parameters.count()<id+1) || (parameters[id].type != aptInteger));
        file_->setFlag(LH_FLAG_HIDDEN, (parameters.count()<id+1) || (parameters[id].type != aptFile));

        if (parameters.count()>=id+1)
        {
            if(parameters[id].type == aptInteger)
            {
                switch (parameters[id].limitType)
                {
                case altWait:    setIntMinMax(int_,0,300000); break;
                case altCursorX: setIntMinMax(int_,cd.range.x.min,cd.range.x.max); break;
                case altCursorY: setIntMinMax(int_,cd.range.y.min,cd.range.y.max); break;
                }
            }

            desc_->setValue(parameters[id].description);
            if(e.tagName()!="")
            {
                QString val = getParameter(e,id);
                str_->setValue(parameters[id].type != aptString ? "" : val);
                int_->setValue(parameters[id].type != aptInteger? id : val.toInt());
                QString val2 = QString("%1%2").arg(lcdhost_state()->dir_layout).arg(val);
                file_->setValue(parameters[id].type != aptFile? QFileInfo() : (QFileInfo(val2).exists()? QFileInfo(val2) : QFileInfo(val)));
            }
        }

    }

    QString getParameterValue(int id, LH_Qt_QString *str_, LH_Qt_int *int_, LH_Qt_QFileInfo *file_)
    {
        if (id>=parameters.count())
            return "";
        switch(parameters[id].type)
        {
        case aptString:
            return str_->value();
        case aptInteger:
            return QString("%1").arg(int_->value());
        case aptFile:
            QString path = file_->value().absoluteFilePath();
            if (path.startsWith(lcdhost_state()->dir_layout))
                path = path.replace(lcdhost_state()->dir_layout,"",Qt::CaseInsensitive);
            return path;
        }
        return "";
    }
};

class actionTypes
{
    QHash<QString,actionType> actionTypes_;
    QStringList actionTypeIDs_;
private:
    actionType add(actionType at)
    {
        actionTypes_.insert(at.typeCode, at);
        actionTypeIDs_.append(at.typeCode);
        return actionTypes_[at.typeCode];
    }

public:
    actionTypes() {
        add( actionType("open"      ,"Open Layout"               , QList<actionParameter>() << actionParameter("Layout File",aptFile) ) );
        add( actionType("run"       ,"Run Application"           , QList<actionParameter>() << actionParameter("Application",aptFile,"path") << actionParameter("Parameters",aptString,"args") ) );
        add( actionType("url"       ,"Open URL"                  , QList<actionParameter>() << actionParameter("URL",aptString) ) );
        add( actionType("move"      ,"Move Cursor"               , QList<actionParameter>() << actionParameter("New X Coordinate",aptInteger,"","x",altCursorX) << actionParameter("New Y Coordinate",aptInteger,"","y",altCursorY)) );
        add( actionType("select"    ,"Move Cursor & Select"      , QList<actionParameter>() << actionParameter("New X Coordinate",aptInteger,"","x",altCursorX) << actionParameter("New Y Coordinate",aptInteger,"","y",altCursorY)) );
        add( actionType("wait"      ,"Wait"                      , QList<actionParameter>() << actionParameter("Delay (in ms)",aptInteger,"","",altWait)) );
        add( actionType("deselect"  ,"Clear Selection") );
        add( actionType("deactivate","Deactivate the Cursor") );
        add( actionType("reselect"  ,"Reselect the previous item") );
    }
    actionType at(int index)
    {
        return at(actionTypeIDs_[index]);
    }
    actionType at(QString typeCode)
    {
        return actionTypes_[typeCode];
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

class LH_CursorAction : public LH_CursorInstance
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
protected:
    QString statusCode_;

    LH_Qt_QString *setup_coordinate_;
    LH_Qt_InputState *setup_jump_to_;

    LH_Qt_QStringList *setup_actions_;
    LH_Qt_QString *setup_action_desc_;
    LH_Qt_QStringList *setup_action_type_;

    LH_Qt_QString *setup_action_parameter1_desc_;
    LH_Qt_QString *setup_action_parameter1_str_;
    LH_Qt_int *setup_action_parameter1_int_;
    LH_Qt_QFileInfo *setup_action_parameter1_file_;

    LH_Qt_QString *setup_action_parameter2_desc_;
    LH_Qt_QString *setup_action_parameter2_str_;
    LH_Qt_int *setup_action_parameter2_int_;
    LH_Qt_QFileInfo *setup_action_parameter2_file_;

    LH_Qt_int *setup_action_index_;
    LH_Qt_bool *setup_action_enabled_;

    LH_Qt_QString *setup_action_add_;
    LH_Qt_QString *setup_action_delete_;
    LH_Qt_QString *setup_action_delete_confirm_;

    LH_Qt_QTextEdit *setup_actions_xml_;
public:
    LH_CursorAction( const char *name, LH_QtPlugin *parent = 0 );

    int polling();

    static lh_class *classInfo();

    void fire(int = 0);

public slots:
    void doJumpTo(QString key,int flags,int value);
    void xmlChanged();
    void actionSelected();
    void actionEdited();
    void actionMoved();
    void newAction();
    void deleteAction();
    void deleteActionCheck();
};

#endif // LH_CURSORACTION_H
