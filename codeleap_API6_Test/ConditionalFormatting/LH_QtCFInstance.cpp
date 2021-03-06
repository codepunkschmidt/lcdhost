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

        const int LH_FLAG_UI = lh_meta_enabled | lh_meta_show;

        cf_initialized_ = true;
        cf_rule_editing_ = None;
        watching_non_setup_item_ = false;

        new LH_Qt_html(this, "<hr>", LH_FLAG_LAST | LH_FLAG_UI );

        setup_cf_enabled_ = new LH_Qt_bool(this, "Enable Conditional Formatting", false, LH_FLAG_LAST | LH_FLAG_AUTORENDER | LH_FLAG_INDENTTITLE);
        setup_cf_enabled_->setHelp("<p>Conditional Formatting allows a number of properties on the object to change automatically.</p><p>E.g. a text object could change it's fore or background colour or its font.</p>");

        new LH_Qt_html(this,"<span style='color:grey'>(Conditional Formatting is still experimental)</span>", LH_FLAG_LAST | LH_FLAG_UI | LH_FLAG_INDENTTITLE );

        setup_cf_state_ = new LH_Qt_QString(this, "State", "", LH_FLAG_NOSAVE | LH_FLAG_READONLY | LH_FLAG_LAST | LH_FLAG_HIDDEN);
        setup_cf_state_->setHelp("<p>One way to simplify the Conditional Formatting rules is to have one set of rules that set this \"State\" value and another set that change colours, fonts, images, etc based on it.</p>");

        setup_cf_visibility_ = new LH_Qt_bool(this, "Visibility", true, LH_FLAG_NOSAVE | LH_FLAG_LAST | LH_FLAG_HIDDEN );

        setup_cf_menu1_ = new LH_Qt_html(this, 0, LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE );
        setup_cf_menu1_->setTitle(
                    menu_area.arg("left").arg(
                        menu_txt.arg("on").arg("a").arg("copy").arg("Copy Rules") +
                        menu_txt.arg("on").arg("a").arg("paste").arg("Paste Rules")
                    ));
        connect( setup_cf_menu1_, SIGNAL(stringChanged(QString)), this, SLOT(cf_menu(QString)) );

        setup_cf_rules_ = new LH_Qt_QStringList(this, "Conditions", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_HIDETITLE, lh_type_string_listbox);

        setup_cf_menu2_ = new LH_Qt_html(this, 0, LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE );
        setRuleItemMenu(false,false,false);
        connect( setup_cf_menu2_, SIGNAL(stringChanged(QString)), this, SLOT(cf_menu(QString)) );

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

        setup_cf_testValue1_ = new LH_Qt_QString(this, "Value 1", "", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_testValue1_List_= new LH_Qt_QStringList(this, "Value 1 (List)", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_testValue2_ = new LH_Qt_QString(this, "Value 2", "", LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_target_ = new LH_Qt_QStringList(this, "Target", QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN);

        setup_cf_newValue_Color_ = new LH_Qt_QColor(this, "New Value - Color",QColor(0,0,0),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_Font_  = new LH_Qt_QFont(this, "New Value - Font",QFont("Arial",10),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_String_= new LH_Qt_QString(this, "New Value - String","",LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_Bool_  = new LH_Qt_bool(this, "New Value - Boolean",false,LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_Int_   = new LH_Qt_int(this, "New Value - Integer",0,LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_File_  = new LH_Qt_QFileInfo(this,"New Value - File",QFileInfo(""),LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_Slider_= new LH_Qt_QSlider(this,"New Value - Slider",0,0,0, LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);
        setup_cf_newValue_List_  = new LH_Qt_QStringList(this,"New Value - List",QStringList(), LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE | LH_FLAG_HIDETITLE);

        setup_cf_menu3_ = new LH_Qt_html(this, 0, LH_FLAG_UI | LH_FLAG_LAST | LH_FLAG_HIDDEN | LH_FLAG_INDENTTITLE );
        setRuleEditMenu(false,false,false);
        connect( setup_cf_menu3_, SIGNAL(stringChanged(QString)), this, SLOT(cf_menu(QString)) );

        setup_cf_XML_ = new LH_Qt_QTextEdit(this, "Conditions XML", "<rules/>", LH_FLAG_LAST | LH_FLAG_HIDDEN);

        connect(setup_cf_enabled_,         SIGNAL(valueChanged()), this, SLOT(cf_enabled_changed()));
        connect(setup_cf_enabled_,         SIGNAL(valueSet()),     this, SLOT(cf_enabled_changed()));
        connect(setup_cf_source_,          SIGNAL(valueChanged()), this, SLOT(cf_source_changed()));
        connect(setup_cf_source_,          SIGNAL(valueSet()),     this, SLOT(cf_source_changed()));
        connect(setup_cf_source_mode_,     SIGNAL(valueChanged()), this, SLOT(cf_source_changed()));
        connect(setup_cf_test_,            SIGNAL(valueChanged()), this, SLOT(cf_condition_changed()));
        connect(setup_cf_test_,            SIGNAL(valueSet()),     this, SLOT(cf_condition_changed()));
        connect(setup_cf_target_,          SIGNAL(valueChanged()), this, SLOT(cf_target_changed()));
        connect(setup_cf_target_,          SIGNAL(valueSet()),     this, SLOT(cf_target_changed()));
        connect(setup_cf_XML_,             SIGNAL(valueChanged()), this, SLOT(cf_XML_changed()));
        connect(setup_cf_rules_,           SIGNAL(valueChanged()), this, SLOT(cf_rules_changed()));

        connect(setup_cf_state_,           SIGNAL(valueChanged()), this, SLOT(cf_state_value_updated()));
        connect(setup_cf_state_,           SIGNAL(valueSet()),     this, SLOT(cf_state_value_updated()));
        connect(setup_cf_visibility_,      SIGNAL(valueChanged()), this, SLOT(cf_update_visibility()));
        connect(setup_cf_visibility_,      SIGNAL(valueSet()),     this, SLOT(cf_update_visibility()));

        connect(setup_cf_source_,          SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_source_mode_,     SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_target_,          SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_test_,            SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_testValue1_,      SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_testValue1_List_, SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_testValue2_,      SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));

        connect(setup_cf_newValue_Color_,  SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_Font_,   SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_String_, SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_Bool_,   SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_Int_,    SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_Slider_, SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_List_,   SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));
        connect(setup_cf_newValue_File_,   SIGNAL(valueChanged()), this, SLOT(cf_rule_edited()));

        cf_source_list_pos = 0;
        cf_target_list_pos = 0;

        add_cf_source(setup_cf_state_, true);
        add_cf_target(setup_cf_state_, true);
        add_cf_target(setup_cf_visibility_, true, true);

        QTimer::singleShot(100, this, SLOT(cf_apply_rules()));
    }
}

void LH_QtCFInstance::add_cf_source(LH_QtSetupItem *si)
{
    add_cf_source(si, false);
}

void LH_QtCFInstance::add_cf_source(QString name)
{
    add_cf_source(name, NULL, false);
}

void LH_QtCFInstance::add_cf_source(LH_QtSetupItem *si, bool atEnd)
{
    //add_cf_source(si->ident(), atEnd);
    add_cf_source(si->ident(), si, atEnd); // JLI: Twice!? Or should this be with title() ?
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

    QStringList sl = setup_cf_source_->list();
    if(atEnd || sources_.count() == 0)
        sl.append(name);
    else
    {
        sl.insert(cf_source_list_pos, name);
        cf_source_list_pos++;
    }

    sources_.insert(new cf_source(this, name, si));
    setup_cf_source_->setList(sl);

    if(si==NULL)
        watching_non_setup_item_ = true;
    else
    {
        sources_[name]->setValue();
        if(si!=setup_cf_state_)
        {
            connect(si, SIGNAL(valueChanged()), this, SLOT(cf_apply_rules()));
            connect(si, SIGNAL(valueSet()), this, SLOT(cf_apply_rules()));
        }
    }
}

void LH_QtCFInstance::add_cf_target(LH_QtSetupItem *si, bool hide)
{
    add_cf_target(si, hide, false);
}

void LH_QtCFInstance::add_cf_target(LH_QtSetupItem *si, bool hide, bool atEnd)
{
    if(hide)
        si->setHidden( true);

    cf_initialize();

    QStringList sl( setup_cf_target_->list() );
    if(atEnd || targets_.length()==0)
    {
        targets_.append(si);
        sl.append(si->ident());
    }
    else
    {
        targets_.insert(cf_target_list_pos, si);
        sl.insert(cf_target_list_pos, si->ident());
        cf_target_list_pos++;
    }
    setup_cf_target_->setList(sl);
}

void LH_QtCFInstance::cf_enabled_changed()
{
    setup_cf_rules_->setHidden( !setup_cf_enabled_->value());

    setup_cf_menu1_->setHidden( !setup_cf_enabled_->value());
    setup_cf_menu2_->setHidden( !setup_cf_enabled_->value());

    setup_cf_state_->setHidden( !setup_cf_enabled_->value());

    cf_set_edit_controls_visibility();

    cf_apply_rules();
    callback(lh_cb_notify, NULL);
}

void LH_QtCFInstance::cf_set_edit_controls_visibility(cf_rule_edit_mode editMode)
{
    if(editMode!=Default)
        cf_rule_editing_ = editMode;

    setup_cf_source_->setHidden( !setup_cf_enabled_->value() || cf_rule_editing_==None);
    setup_cf_test_->setHidden( !setup_cf_enabled_->value() || cf_rule_editing_==None);
    if(!setup_cf_enabled_->value() || cf_rule_editing_==None)
    {
        setup_cf_testValue1_->setHidden( true);
        setup_cf_testValue1_List_->setHidden( true);
    }
    setup_cf_target_->setHidden( !setup_cf_enabled_->value() || cf_rule_editing_==None);

    setRuleItemMenu(cf_rule_editing_==Existing,cf_rule_editing_==Existing,cf_rule_editing_==Existing);
    setRuleEditMenu(setup_cf_enabled_->value() && cf_rule_editing_!=None, rule_dirty_, true);

    cf_source_changed();
    cf_condition_changed();
    cf_target_changed();
}

void LH_QtCFInstance::cf_source_changed()
{
    if (setup_cf_source_->index()>=0)
    {
        setup_cf_source_mode_->setHidden( !setup_cf_enabled_->value() || cf_rule_editing_==None );

        int source_type = (setup_cf_source_mode_->value()=="Value"? sources_[setup_cf_source_->value()]->type() : lh_type_string);
        bool v1vis = (!setup_cf_testValue1_->isHidden()) || (!setup_cf_testValue1_List_->isHidden());
        bool isList = source_type==lh_type_string_dropdownbox || source_type==lh_type_string_listbox;
        setup_cf_testValue1_->setHidden( (!isList? !v1vis : true));
        setup_cf_testValue1_List_->setHidden( (isList? !v1vis : true));

        if(isList)
        {
            LH_Qt_QStringList* source_List = (LH_Qt_QStringList*)sources_[setup_cf_source_->value()]->obj();
            setup_cf_testValue1_List_->setList(source_List->list());
        }

        if( sources_.contains(setup_cf_source_->value()) )
            setup_cf_source_mode_->setList(sources_[setup_cf_source_->value()]->getModes());
    }
}

void LH_QtCFInstance::cf_target_changed()
{
    if( !targets_.contains(setup_cf_target_->value()) ) return;
    lh_setup_type targetType = targets_[setup_cf_target_->value()]->type();

    if(targetType==lh_type_string_listbox) targetType=lh_type_string_dropdownbox;

    setup_cf_newValue_Color_->setFlag( LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_Color_->type() || cf_rule_editing_==None);
    setup_cf_newValue_Font_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_Font_->type()  || cf_rule_editing_==None);
    setup_cf_newValue_String_->setHidden( !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_String_->type()|| cf_rule_editing_==None);
    setup_cf_newValue_Bool_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_Bool_->type()  || cf_rule_editing_==None);
    setup_cf_newValue_Int_->setFlag(   LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_Int_->type()   || cf_rule_editing_==None);
    setup_cf_newValue_File_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_File_->type()  || cf_rule_editing_==None);
    setup_cf_newValue_Slider_->setHidden( !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_Slider_->type()|| cf_rule_editing_==None);
    setup_cf_newValue_List_->setFlag(  LH_FLAG_HIDDEN, !setup_cf_enabled_->value() || targetType!=setup_cf_newValue_List_->type()  || cf_rule_editing_==None);

    if(targetType==setup_cf_newValue_Slider_->type())
    {
        LH_Qt_QSlider* target = (LH_Qt_QSlider*)targets_[setup_cf_target_->value()];
        setup_cf_newValue_Slider_->setMin(target->min());
        setup_cf_newValue_Slider_->setMax(target->max());
    }
    if(targetType==setup_cf_newValue_List_->type())
    {
        LH_Qt_QStringList* target = (LH_Qt_QStringList*)targets_[setup_cf_target_->value()];
        setup_cf_newValue_List_->setList(target->list());
    }
}

void LH_QtCFInstance::cf_condition_changed()
{
    QString conditionText = setup_cf_test_->value();
    lh_setup_type source_type = (setup_cf_source_mode_->value()=="Value"? sources_[setup_cf_source_->value()]->type() : lh_type_string);
    bool isList = source_type==lh_type_string_dropdownbox || source_type==lh_type_string_listbox;
    setup_cf_testValue1_->setHidden( (isList? true : !setup_cf_enabled_->value() || cf_rule_editing_==None));
    setup_cf_testValue1_List_->setHidden( (!isList? true : !setup_cf_enabled_->value() || cf_rule_editing_==None));
    setup_cf_testValue2_->setHidden( !setup_cf_enabled_->value() || !conditionText.contains("etween") || cf_rule_editing_==None);
}

void LH_QtCFInstance::cf_cancel_edit_rule()
{
    setup_cf_rules_->setValue(QString());
    cf_set_edit_controls_visibility(None);
}

void LH_QtCFInstance::cf_XML_changed()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    int selIndex = setup_cf_rules_->index();
    if(cf_rule_editing_!=Existing) selIndex = -1;

    QStringList sl;
    for(uint i=0; i<root.childNodes().length(); i++)
    {
        cf_rule rule(root.childNodes().at(i));
        sl.append(rule.description());
    }
    setup_cf_rules_->setList(sl);
    setup_cf_rules_->setIndex(selIndex);
    setup_cf_rules_->setHidden( !setup_cf_enabled_->value());
    cf_apply_rules();
}

void LH_QtCFInstance::cf_rules_changed()
{
    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    bool validSel = setup_cf_rules_->index()>=0 && setup_cf_rules_->index() < (int)root.childNodes().length();
    if(validSel)
    {
        cf_source_changed();
        cf_set_edit_controls_visibility(Existing);

        cf_rule rule(root.childNodes().at(setup_cf_rules_->index()));
        rule_dirty_ = false;
        rule.conditions[0]->edit(this, sources_);
        rule.actions[0]->edit(this, targets_);
    } else
        cf_set_edit_controls_visibility(None);

    setRuleItemMenu(validSel,validSel,validSel);
}

void LH_QtCFInstance::cf_delete_rule()
{
    if(setup_cf_rules_->index()==-1)
        return;

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());

    QDomNodeList rules = doc.firstChild().childNodes();
    if( setup_cf_rules_->index() >= 0 && setup_cf_rules_->index() < (int)rules.length() )
    {
        QDomNode n = rules.at(setup_cf_rules_->index());
        doc.firstChild().removeChild(n);
        setup_cf_XML_->setValue(doc.toString());
        cf_XML_changed();
        cf_rules_changed();
    }
}

void LH_QtCFInstance::cf_new_rule()
{
    setup_cf_rules_->setIndex(-1);
    cf_set_edit_controls_visibility(New);

    rule_dirty_ = true;
    setup_cf_source_->setValue("");
    setup_cf_test_->setValue("");
    setup_cf_testValue1_->setValue("");
    setup_cf_testValue2_->setValue("");
    setup_cf_target_->setValue("");
    cf_target_changed();
}

void LH_QtCFInstance::cf_state_value_updated()
{
    if (QObject::sender()!=NULL)
    {
        QString senderName = ((LH_QtSetupItem*)QObject::sender())->ident();
        if(sources_.contains(senderName))
            sources_[senderName]->setValue();
    }
}

void LH_QtCFInstance::cf_update_visibility()
{
    setVisible(setup_cf_visibility_->value());
}

void LH_QtCFInstance::cf_apply_rules(bool allowRender)
{
    if(!setup_cf_enabled_->value())
        return;

    if (QObject::sender()!=NULL && QObject::sender()->inherits("LH_QtSetupItem"))
    {
        LH_QtSetupItem* sender_setup_item = ((LH_QtSetupItem*)QObject::sender());
        if(sender_setup_item->ident()!=NULL)
        {
            QString senderName = sender_setup_item->ident();
            if(sources_.contains(senderName))
                sources_[senderName]->setValue();
        }
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
    QFile file( dir_data().append("cf_cache.xml") );
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
    QFile file( dir_data().append("cf_cache.xml") );
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

void LH_QtCFInstance::cf_move_rule(bool up)
{
    int i = setup_cf_rules_->index();

    QDomDocument doc("");
    doc.setContent(setup_cf_XML_->value());
    QDomElement root = doc.firstChild().toElement();

    if(( up && i>=1 && i < (int)root.childNodes().length()) ||
       (!up && i>=0 && i < (int)root.childNodes().length()-1))
    {
        if(up)
            root.insertBefore(root.childNodes().at(i),root.childNodes().at(i-1));
        else
            root.insertAfter( root.childNodes().at(i),root.childNodes().at(i+1));
        setup_cf_rules_->setIndex(i+(up?-1:1));
        setup_cf_XML_->setValue(doc.toString());
        cf_XML_changed();
        cf_rules_changed();
    }

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

    if(setup_cf_rules_->index()==-1)
        doc.firstChild().appendChild(ruleNode);
    else
        doc.firstChild().replaceChild(ruleNode, doc.firstChild().childNodes().at(setup_cf_rules_->index()));

    setup_cf_XML_->setValue(doc.toString());
    rule_dirty_ = false;
    setRuleEditMenu(setup_cf_enabled_->value(), rule_dirty_, true);
    cf_XML_changed();
}

void LH_QtCFInstance::cf_menu(QString s)
{
    if( s == "copy" ) cf_copy_rules();
    else if( s == "paste" ) cf_paste_rules();
    else if( s == "up" ) cf_move_rule(true);
    else if( s == "down" ) cf_move_rule(false);
    else if( s == "new" ) cf_new_rule();
    else if( s == "delete" ) cf_delete_rule();
    else if( s == "save" ) cf_save_rule();
    else if( s == "cancel" ) cf_cancel_edit_rule();
    return;
}

void LH_QtCFInstance::setRuleItemMenu(bool Enable_Up, bool Enable_Down, bool Enable_Delete)
{
    Q_UNUSED(Enable_Up);
    Q_UNUSED(Enable_Down);
    Q_UNUSED(Enable_Delete);
    bool atTop = setup_cf_rules_->index()==0;
    bool atBottom = setup_cf_rules_->index()==setup_cf_rules_->list().count()-1;

    Enable_Up = Enable_Up && !atTop;
    Enable_Down = Enable_Down && !atBottom;

    setup_cf_menu2_->setTitle(
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
    setup_cf_menu3_->setHidden( !visible);
    setup_cf_menu3_->setTitle(
                menu_area.arg("right").arg(
                    menu_txt.arg(Enable_Save?   "on" : "off").arg(Enable_Save?   "a" : "span").arg("save").arg("&nbsp;Save&nbsp;") +
                    menu_txt.arg(Enable_Cancel? "on" : "off").arg(Enable_Cancel? "a" : "span").arg("cancel").arg("Cancel")
                )
      );
}

void LH_QtCFInstance::cf_rule_edited()
{
    rule_dirty_ = true;
    setRuleEditMenu(setup_cf_enabled_->value(), rule_dirty_, true);
}

void LH_QtCFInstance::cf_set_rules(QString rulesXML, bool enable_cf)
{
    setup_cf_XML_->setValue(rulesXML);
    if(enable_cf)
        setup_cf_enabled_->setValue(true);
    cf_XML_changed();
    cf_rules_changed();
}
