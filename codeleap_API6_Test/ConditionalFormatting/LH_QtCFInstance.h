/**
  \file     LH_QtCFInstance.h
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

#ifndef LH_QTCFINSTANCE_H
#define LH_QTCFINSTANCE_H

#include <QtGlobal>
#include <QImage>
#include <QtXml>
#include <QTimer>

#include "LH_QtPlugin.h"
#include "LH_QtObject.h"
#include "LH_QtInstance.h"
#include "LH_Qt_bool.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_html.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_QTextEdit.h"
#include "LH_Qt_QColor.h"
#include "LH_Qt_QFont.h"
#include "LH_Qt_QFileInfo.h"
#include "LH_Qt_QSlider.h"
#include "LH_Qt_int.h"
#include "LH_Qt_html.h"

#include "cf_sources_targets.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif

#define USE_MENUS

class cf_rule;

static const QString menu_area = "<style type='text/css'>"
                                    "  a {text-decoration:none; color: #000000; }"
                                    "  table.btn {border-color:black; border-style:solid; border-width:1px;}"
                                    "  td.on {background-color:#E0E0E0;"
                                    "         padding-left: 4px; padding-right: 4px; vertical-align:middle} "
                                    "  td.off {background-color:#F0F0F0; color:#606060;"
                                    "         padding-left: 4px; padding-right: 4px; vertical-align:middle} "
                                    "</style>"
                                    "<table cellspacing='4' style='float:%1'><tr>%2</tr></table>";
static const QString menu_img = "<td><table cellspacing='1' cellpadding='4' class='btn'><tr><td class='%1'><%2 href='%3'><img src=':/cf/%3_%1.png'/></%1></td></tr></table></td>";
static const QString menu_txt = "<td><table cellspacing='1' cellpadding='4' class='btn'><tr><td class='%1'>"
                                    "<table cellspacing='0' cellpadding='0'><tr>"
                                    "<td><%2 href='%3'><img src=':/cf/%3_%1.png'/></%1></td>"
                                    "<td><%2 href='%3'>&nbsp;%4</%1></td>"
                                    "</tr></table>"
                                "</td></tr></table></td>";

class LH_QtCFInstance : public LH_QtInstance
{
    Q_OBJECT

    enum cf_rule_edit_mode
    {
        Default = -1,
        None = 0,
        Existing = 1,
        New = 2
    };

    cf_source_list sources_;
    cf_target_list targets_;

    int cf_source_list_pos;
    int cf_target_list_pos;

    //QDomElement createNode(QDomDocument doc, QString tagName, QString nodeValue = "", QString attributeName = "", QString attributeValue = "");

    bool cf_initialized_;
    void cf_initialize();

    bool rule_dirty_;

    void add_cf_source(QString name, LH_QtSetupItem *si, bool atEnd = false);

    //QString getTargetValue(int targetId);

    cf_rule_edit_mode cf_rule_editing_;
    void cf_set_edit_controls_visibility(cf_rule_edit_mode editMode = Default);

    bool watching_non_setup_item_;

    void setRuleItemMenu(bool, bool, bool);
    void setRuleEditMenu(bool, bool, bool);

    void add_cf_source(LH_QtSetupItem *si, bool atEnd);
    void add_cf_source(QString name, bool atEnd);
    void add_cf_target(LH_QtSetupItem *s, bool hide, bool atEnd);
protected:
    LH_Qt_bool        *setup_cf_enabled_;

    LH_Qt_QString     *setup_cf_state_;
    LH_Qt_bool        *setup_cf_visibility_;

    LH_Qt_QTextEdit   *setup_cf_XML_;
    LH_Qt_QStringList *setup_cf_rules_;

    LH_Qt_html        *setup_cf_menu1_;
    LH_Qt_html        *setup_cf_menu2_;
    LH_Qt_html        *setup_cf_menu3_;

    void cf_source_notify(QString name, QString value, int index = 0, int count = 1);

    void add_cf_source(LH_QtSetupItem *si);
    void add_cf_source(QString name);
    void add_cf_target(LH_QtSetupItem *si, bool hide = false);

    void cf_set_rules(QString rulesXML, bool enable_cf = true);
    void cf_move_rule(bool up);
public:
    LH_Qt_QStringList *setup_cf_source_;
    LH_Qt_QStringList *setup_cf_source_mode_;
    LH_Qt_QStringList *setup_cf_test_;
    LH_Qt_QString     *setup_cf_testValue1_;
    LH_Qt_QStringList *setup_cf_testValue1_List_;
    LH_Qt_QString     *setup_cf_testValue2_;
    LH_Qt_QStringList *setup_cf_target_;
    LH_Qt_QColor      *setup_cf_newValue_Color_;
    LH_Qt_QFont       *setup_cf_newValue_Font_;
    LH_Qt_QString     *setup_cf_newValue_String_;
    LH_Qt_QFileInfo   *setup_cf_newValue_File_;
    LH_Qt_bool        *setup_cf_newValue_Bool_;
    LH_Qt_int         *setup_cf_newValue_Int_;
    LH_Qt_QSlider     *setup_cf_newValue_Slider_;
    LH_Qt_QStringList *setup_cf_newValue_List_;

    LH_QtCFInstance() :
        LH_QtInstance(),
        cf_initialized_(false),
        cf_rule_editing_(Default),
        watching_non_setup_item_(false),
        setup_cf_source_(0),
        setup_cf_source_mode_(0),
        setup_cf_test_(0),
        setup_cf_testValue1_(0),
        setup_cf_testValue1_List_(0),
        setup_cf_testValue2_(0),
        setup_cf_target_(0),
        setup_cf_newValue_Color_(0),
        setup_cf_newValue_Font_(0),
        setup_cf_newValue_String_(0),
        setup_cf_newValue_File_(0),
        setup_cf_newValue_Bool_(0),
        setup_cf_newValue_Int_(0),
        setup_cf_newValue_Slider_(0),
        setup_cf_newValue_List_(0)
    {}

    int notify(int n,void* p);

    cf_source_list sources() { return sources_; }
    cf_target_list targets() { return targets_; }

protected slots:
    void cf_enabled_changed();
    void cf_source_changed();
    void cf_target_changed();
    void cf_condition_changed();
    void cf_save_rule();
    void cf_cancel_edit_rule();
    void cf_XML_changed();
    void cf_rules_changed();
    void cf_delete_rule();
    void cf_new_rule();
    void cf_state_value_updated();
    void cf_update_visibility();
    void cf_apply_rules(bool allowRender = true);
    void cf_rule_edited();
    void cf_copy_rules();
    void cf_paste_rules();
    void cf_menu(QString);
};

#endif // LH_QTCFINSTANCE_H
