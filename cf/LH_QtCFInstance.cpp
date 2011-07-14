/**
  \file     LH_QtCFInstance.cpp
  \author   Andy Bridges <triscopic@codeleap.co.uk>
  \legalese Copyright (c) 2010-2011 Andy Bridges

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#include <QDebug>
#include "LH_QtCFInstance.h"
#include "cf_rule.h"


/**
  Conditional Formatting Routines

  to add conditional formatting to an object simply
  add source objects and target objects:

  \code
    add_cf_source(setup_text_);
    add_cf_target(setup_pencolor_);
    add_cf_target(setup_bgcolor_);
    add_cf_target(setup_font_);
  \endcode

  Source objects are watched for chagnes and their
  new values tested against the specified rules.

  Target objects are changed when rules are met.
  */

int LH_QtCFInstance::notify(int n,void* p)
{
    Q_UNUSED(p);
    if( n&LH_NOTE_SECOND )
        cf_apply_rules();
    return (watching_non_setup_item_ && setup_cf_enabled_->value()? LH_NOTE_SECOND : 0) | LH_QtInstance::notify(n, p);
}

void LH_QtCFInstance::cf_initialize()
{
    if (cf_initialized_)
        return;
    else
    {
        rule_dirty_ = false;

        int LH_FLAG_UI = LH_FLAG_NOSAVE | LH_FLAG_NOSOURCE | LH_FLAG_NOSINK ;

        cf_initialized_ = true;
        cf_rule_editing_ = None;
        watching_non_setup_item_ = false;

        LH_Qt_QString *hr = new LH_Qt_QString(this,tr("~CF-Area-Rule"),QString(),LH_FLAG_LAST | LH_FLAG_UI, lh_type_string_htmlhelp );
        hr->setHelp("<hr>");

        setup_cf_enabled_ = new LH_Qt_bool(this, "^Enable Conditional Formatting", false, LH_FLAG_LAST | LH_FLAG_AUTORENDER);
        setup_cf_enabled_->setHelp("<p>Conditional Formatting allows a number of properties on the object to change automatically.</p><p>E.g. a text object could change it's fore or background colour or its font.</p>");

        LH_Qt_QString *comment = new LH_Qt_QString(this,tr("^comment"), QString(), LH_FLAG_LAST | LH_FLAG_UI,lh_type_string_htmlhelp );
        comment->setHelp("<span style='color:grey'>(Conditional Formatting is still experimental)</span>");

        setup_cf_state_ = new LH_Qt_QString(this, "State", "", LH_FLAG_NOSAVE | LH_FLAG_READONLY | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_state_->setHelp("<p>One way to simplify the Conditional Formatting rules is to have one set of rules that set this \"State\" value and another set that change colours, fonts, images, etc based on it.</p>");

        setup_cf_visibility_ = new LH_Qt_bool(this, "Visibility", true, LH_FLAG_NOSAVE | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_menu1_ = new LH_Qt_QString(this, "^CFMenu1", QString(),  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_htmlhelp);
        setup_cf_menu1_->setHelp(
                    menu_area.arg("left").arg(
                        menu_txt.arg("on").arg("a").arg("copy").arg("Copy Rules") +
                        menu_txt.arg("on").arg("a").arg("paste").arg("Paste Rules")
                    ));
        connect( setup_cf_menu1_, SIGNAL(change(QString)), this, SLOT(cf_menu(QString)) );

        setup_cf_rules_ = new LH_Qt_QStringList(this, "^Conditions", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_integer_listbox);

        setup_cf_menu2_ = new LH_Qt_QString(this, "^CFMenu2", QString(),  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_htmlhelp);
        setRuleItemMenu(false,false,false);
        connect( setup_cf_menu2_, SIGNAL(change(QString)), this, SLOT(cf_menu(QString)) );

        setup_cf_source_ = new LH_Qt_QStringList(this, "Source", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_source_mode_ = new LH_Qt_QStringList(this, "Value", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_test_ = new LH_Qt_QStringList(this, "Condition",
                                                    QStringList()
                                                    << "Equals"       << "Does not equal"
                                                    << "Less than"    << "Less than or equal"
                                                    << "Greater than" << "Greater than or equal"
                                                    << "Between"      << "Is not between"
                                                    << "Begins with"  << "Does not begin with"
                                                    << "Ends with"    << "Does not end with"
                                                    << "Contains"     << "Does not contain"
                                                    , LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_testValue1_ = new LH_Qt_QString(this, "^Value 1", "", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_testValue2_ = new LH_Qt_QString(this, "^Value 2", "", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_target_ = new LH_Qt_QStringList(this, "Target", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_newValue_Color_ = new LH_Qt_QColor(this, "^New Value - Color",QColor(0,0,0),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_Font_  = new LH_Qt_QFont(this, "^New Value - Font",QFont("Arial",10),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_String_= new LH_Qt_QString(this, "^New Value - String","",LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_Bool_ = new LH_Qt_bool(this, "^New Value - Boolean","",LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_newValue_File_  = new LH_Qt_QFileInfo(this,"^New Value - File",QFileInfo(""),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_menu3_ = new LH_Qt_QString(this, "^CFMenu3", QString(),  LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN, lh_type_string_htmlhelp);
        setRuleEditMenu(false,false,false);
        connect( setup_cf_menu3_, SIGNAL(change(QString)), this, SLOT(cf_menu(QString)) );

        setup_cf_XML_ = new LH_Qt_QTextEdit(this, "Conditions XML", "<rules/>", LH_FLAG_LAST | LH_FLAG_HIDDEN);

        connect(setup_cf_enabled_, SIGNAL(changed()), this, SLOT(cf_enabled_changed()));
        connect(setup_cf_source_, SIGNAL(changed()), this, SLOT(cf_source_changed()));
        connect(setup_cf_source_, SIGNAL(set()), this, SLOT(cf_source_changed()));
        connect(setup_cf_target_, SIGNAL(changed()), this, SLOT(cf_target_changed()));
        connect(setup_cf_target_, SIGNAL(set()), this, SLOT(cf_target_changed()));
        connect(setup_cf_test_, SIGNAL(changed()), this, SLOT(cf_condition_changed()));
        connect(setup_cf_test_, SIGNAL(set()), this, SLOT(cf_condition_changed()));
        connect(setup_cf_XML_, SIGNAL(changed()), this, SLOT(cf_XML_changed()));
        connect(setup_cf_rules_, SIGNAL(changed()), this, SLOT(cf_rules_changed()));

        connect(setup_cf_state_, SIGNAL(changed()), this, SLOT(cf_state_value_updated()));
        connect(setup_cf_state_, SIGNAL(set()), this, SLOT(cf_state_value_updated()));
        connect(setup_cf_visibility_, SIGNAL(changed()), this, SLOT(cf_update_visibility()));
        connect(setup_cf_visibility_, SIGNAL(set()), this, SLOT(cf_update_visibility()));

        connect(setup_cf_source_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_target_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_test_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_testValue1_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_testValue2_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));

        connect(setup_cf_newValue_Color_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_Font_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_String_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_Bool_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_File_, SIGNAL(changed()), this, SLOT(cf_rule_edited()));

        cf_source_list_pos = 0;
        cf_target_list_pos = 0;

        add_cf_source(setup_cf_state_, true);
        add_cf_target(setup_cf_state_, true);
        add_cf_target(setup_cf_visibility_, true);
    }
}

void LH_QtCFInstance::add_cf_source(LH_QtSetupItem *si, bool atEnd)
{
    add_cf_source(si->name(), si, atEnd);
}

void LH_QtCFInstance::add_cf_source(QString name, bool atEnd)
{
    add_cf_source(name, NULL, atEnd);
}

void LH_QtCFInstance::add_cf_source(QString name, LH_QtSetupItem* si, bool atEnd)
{
    cf_initialize();
    if(sources_.contains(name))
        return;

    if(atEnd || sources_.count() == 0)
        setup_cf_source_->list().append(name);
    else
    {
        setup_cf_source_->list().insert(cf_source_list_pos, name);
        cf_source_list_pos++;
    }

    sources_.insert(new cf_source(this, name, si));
    setup_cf_source_->refreshList();

    if(si==NULL)
        watching_non_setup_item_ = true;
    else
    {
        sources_[name]->setValue();
        if(si!=setup_cf_state_)
        {
            connect(si, SIGNAL(changed()), this, SLOT(cf_apply_rules()));
            connect(si, SIGNAL(set()), this, SLOT(cf_apply_rules()));
        }
    }
}

void LH_QtCFInstance::add_cf_target(LH_QtSetupItem *si, bool atEnd)
{
    cf_initialize();
    if(atEnd || targets_.length()==0)
    {
        targets_.append(si);
        setup_cf_target_->list().append(si->name());
    }
    else
    {
        targets_.insert(cf_target_list_pos, si);
        setup_cf_target_->list().insert(cf_target_list_pos, si->name());
        cf_target_list_pos++;
    }
    setup_cf_target_->refreshList();
}

void LH_QtCFInstance::cf_enabled_changed()
{
    setup_cf_rules_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());

    setup_cf_menu1_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    setup_cf_menu2_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());

    setup_cf_state_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());

    cf_set_edit_controls_visibility();

    callback(lh_cb_notify, NULL);
}

void LH_QtCFInstance::cf_set_edit_controls_visibility(cf_rule_edit_mode editMode)
{
    if(editMode!=Default)
        cf_rule_editing_ = editMode;

    setup_cf_source_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_test_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_testValue1_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_target_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None);

    setRuleItemMenu(cf_rule_editing_==Existing,cf_rule_editing_==Existing,cf_rule_editing_==Existing);
    setRuleEditMenu(setup_cf_enabled_->value() && cf_rule_editing_!=None, rule_dirty_, true);

    cf_source_changed();
    cf_condition_changed();
    cf_target_changed();
}

void LH_QtCFInstance::cf_source_changed()
{
    if (setup_cf_source_->value()>=0)
    {
        setup_cf_source_mode_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || cf_rule_editing_==None );

        setup_cf_source_mode_->list().clear();
        setup_cf_source_mode_->list().append(sources_[setup_cf_source_->valueText()]->getModes());
        setup_cf_source_mode_->refreshList();
    }
}

void LH_QtCFInstance::cf_target_changed()
{
    if (setup_cf_target_->value()>=0 && setup_cf_target_->value()<targets_.length())
    {
        setup_cf_newValue_Color_->setFlag( LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_Color_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_Font_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_Font_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_String_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_String_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_Bool_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_Bool_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
        setup_cf_newValue_File_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || setup_cf_newValue_File_->type()!=targets_[setup_cf_target_->value()]->type()  || cf_rule_editing_==None);
    }
}

void LH_QtCFInstance::cf_condition_changed()
{
    QString conditionText = setup_cf_test_->valueText();
    setup_cf_testValue2_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || !conditionText.contains("etween") || cf_rule_editing_==None);
}

void LH_QtCFInstance::cf_cancel_edit_rule()
{
    setup_cf_rules_->setValue(-1);
    cf_set_edit_controls_visibility(None);
}

void LH_QtCFInstance::cf_XML_changed()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    int selIndex = setup_cf_rules_->value();
    if(cf_rule_editing_!=Existing) selIndex = -1;
    setup_cf_rules_->list().clear();
    for(uint i=0; i<root.childNodes().length(); i++)
    {
        cf_rule rule(root.childNodes().at(i));
        setup_cf_rules_->list().append(rule.description());
    }
    setup_cf_rules_->refreshList();    
    setup_cf_rules_->setValue(selIndex);
    setup_cf_rules_->setFlag(LH_FLAG_HIDDEN, !setup_cf_enabled_->value());
    cf_apply_rules();
}

void LH_QtCFInstance::cf_rules_changed()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    bool validSel = setup_cf_rules_->value()>=0 && setup_cf_rules_->value() < (int)root.childNodes().length();
    if(validSel)
    {
        cf_source_changed();
        cf_set_edit_controls_visibility(Existing);

        cf_rule rule(root.childNodes().at(setup_cf_rules_->value()));
        rule_dirty_ = false;
        rule.conditions[0]->edit(this);
        rule.actions[0]->edit(this, targets_);
    } else
        cf_set_edit_controls_visibility(None);

    setRuleItemMenu(validSel,validSel,validSel);
}

void LH_QtCFInstance::cf_delete_rule()
{
    if(setup_cf_rules_->value()==-1)
        return;

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());

    QDomNodeList rules = doc.firstChild().childNodes();
    if( setup_cf_rules_->value() >= 0 && setup_cf_rules_->value() < (int)rules.length() )
    {
        QDomNode n = rules.at(setup_cf_rules_->value());
        doc.firstChild().removeChild(n);
        setup_cf_XML_->setValue(doc.toString());
        cf_XML_changed();
        cf_rules_changed();
    }
}

void LH_QtCFInstance::cf_new_rule()
{
    setup_cf_rules_->setValue(-1);
    cf_set_edit_controls_visibility(New);

    rule_dirty_ = true;
    setup_cf_source_->setValue(0);
    setup_cf_test_->setValue(0);
    setup_cf_testValue1_->setValue("");
    setup_cf_testValue2_->setValue("");
    setup_cf_target_->setValue(0);
}

void LH_QtCFInstance::cf_state_value_updated()
{
    if (QObject::sender()!=NULL)
    {
        QString senderName = ((LH_QtSetupItem*)QObject::sender())->name();
        if(sources_.contains(senderName))
            sources_[senderName]->setValue();
    }
}

void LH_QtCFInstance::cf_update_visibility()
{
    if (QObject::sender()!=NULL)
    {
        //QString senderName = ((LH_QtSetupItem*)QObject::sender())->name();
        //if(sources_.contains(senderName))
        //    sources_[senderName]->setValue();
    }
    setVisible(setup_cf_visibility_->value());
}

void LH_QtCFInstance::cf_apply_rules(bool allowRender)
{
    if(!setup_cf_enabled_->value())
        return;

    if (QObject::sender()!=NULL)
    {
        QString senderName = ((LH_QtSetupItem*)QObject::sender())->name();
        if(sources_.contains(senderName))
            sources_[senderName]->setValue();
    }

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    bool doRender = false;

    for(uint i=0; i<root.childNodes().length(); i++)
        doRender |= cf_rule(root.childNodes().at(i)).apply(this, sources_, targets_);

    if(doRender && allowRender) this->requestRender();
}

void LH_QtCFInstance::cf_copy_rules()
{
    QFile file(QString("%1cf_cache.xml").arg(state()->dir_binaries));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    else
        file.write(setup_cf_XML_->value().toAscii());
    /*QClipboard *clipboard = QApplication::clipboard();
    QString newText = setup_cf_XML_->value();
    qDebug() << newText;
    clipboard->clear();*/
}

void LH_QtCFInstance::cf_paste_rules()
{
    QString clip_text;
    QFile file(QString("%1cf_cache.xml").arg(state()->dir_binaries));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    else
        clip_text = QString(file.readAll().data());


    //const QClipboard *clipboard = QApplication::clipboard();
    //const QMimeData *mimeData = clipboard->mimeData();
    //if(!mimeData->hasText())
    //    return;
    //QString clip_text = mimeData->text();

    QDomDocument doc_clip("");
    if(doc_clip.setContent(clip_text))
    {
        QDomElement root_clip = doc_clip.firstChild().toElement();

        QDomDocument doc_curr("");
        doc_curr.setContent(setup_cf_XML_->value());
        QDomElement root_curr = doc_curr.firstChild().toElement();

        QDomNodeList rules = root_clip.elementsByTagName("rule");
        while(rules.length()!=0)
            root_curr.appendChild(rules.at(0));

        setup_cf_XML_->setValue(doc_curr.toString());
        cf_XML_changed();
    }
}

void LH_QtCFInstance::cf_move_rule_up()
{
    int i = setup_cf_rules_->value();

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    if(i>=1 && i < (int)root.childNodes().length())
    {
        root.insertBefore(root.childNodes().at(i),root.childNodes().at(i-1));
        setup_cf_rules_->setValue(i-1);
        cf_rules_changed();
    }

    setup_cf_XML_->setValue(doc.toString());
    cf_XML_changed();
}

void LH_QtCFInstance::cf_move_rule_down()
{
    int i = setup_cf_rules_->value();

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    if(i>=0 && i < (int)root.childNodes().length()-1)
    {
        root.insertAfter(root.childNodes().at(i),root.childNodes().at(i+1));
        setup_cf_rules_->setValue(i+1);
        cf_rules_changed();
    }

    setup_cf_XML_->setValue(doc.toString());
    cf_XML_changed();
}

void LH_QtCFInstance::cf_source_notify(QString name, QString value, int index, int count)
{
    sources_[name]->setValue(value, index);
    if(index==count-1)
        cf_apply_rules(false);    
}

void LH_QtCFInstance::cf_save_rule()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());

    QDomNode ruleNode = cf_rule(this).toXmlNode(doc);

    if(setup_cf_rules_->value()==-1)
        doc.firstChild().appendChild(ruleNode);
    else
        doc.firstChild().replaceChild(ruleNode, doc.firstChild().childNodes().at(setup_cf_rules_->value()));

    setup_cf_XML_->setValue(doc.toString());
    rule_dirty_ = false;
    setRuleEditMenu(true, rule_dirty_, true);
    cf_XML_changed();
}

void LH_QtCFInstance::cf_menu(QString s)
{
    if( s == "copy" ) cf_copy_rules();
    else if( s == "paste" ) cf_paste_rules();
    else if( s == "up" ) cf_move_rule_up();
    else if( s == "down" ) cf_move_rule_down();
    else if( s == "new" ) cf_new_rule();
    else if( s == "delete" ) cf_delete_rule();
    else if( s == "save" ) cf_save_rule();
    else if( s == "cancel" ) cf_cancel_edit_rule();
    return;
}

void LH_QtCFInstance::setRuleItemMenu(bool Enable_Up, bool Enable_Down, bool Enable_Delete)
{
    bool atTop = setup_cf_rules_->value()==0;
    bool atBottom = setup_cf_rules_->value()==setup_cf_rules_->list().count()-1;

    Enable_Up = Enable_Up && !atTop;
    Enable_Down = Enable_Down && !atBottom;

    setup_cf_menu2_->setHelp(
                menu_area.arg("right").arg(
                    menu_txt.arg(true?          "on" : "off").arg(true?          "a" : "span").arg("new").arg("&nbsp;New&nbsp;&nbsp;") +
                    menu_txt.arg(Enable_Delete? "on" : "off").arg(Enable_Delete? "a" : "span").arg("delete").arg("Delete") +
                    menu_img.arg(Enable_Up?     "on" : "off").arg(Enable_Up?     "a" : "span").arg("up") +
                    menu_img.arg(Enable_Down?   "on" : "off").arg(Enable_Down?   "a" : "span").arg("down")
                )
      );
}

void LH_QtCFInstance::setRuleEditMenu(bool visible, bool Enable_Save, bool Enable_Cancel)
{
    setup_cf_menu3_->setFlag(LH_FLAG_HIDDEN, !visible);
    setup_cf_menu3_->setHelp(
                menu_area.arg("right").arg(
                    menu_txt.arg(Enable_Save?   "on" : "off").arg(Enable_Save?   "a" : "span").arg("save").arg("&nbsp;Save&nbsp;") +
                    menu_txt.arg(Enable_Cancel? "on" : "off").arg(Enable_Cancel? "a" : "span").arg("cancel").arg("Cancel")
                )
      );
}

void LH_QtCFInstance::cf_rule_edited()
{
    rule_dirty_ = true;
    setRuleEditMenu(true, rule_dirty_, true);
}
