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

#include "LH_QtPlugin.h"
#include "LH_QtObject.h"
#include "LH_QtInstance.h"

#include "LH_Qt_bool.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QStringList.h"
#include "LH_Qt_QTextEdit.h"
#include "LH_Qt_QColor.h"
#include "LH_Qt_QFont.h"
#include "LH_Qt_QFileInfo.h"

#include "cf_sources_targets.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif

class cf_rule;

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

    //QDomElement createNode(QDomDocument doc, QString tagName, QString nodeValue = "", QString attributeName = "", QString attributeValue = "");

    bool cf_initialized_;
    void cf_initialize();

    void add_cf_source(QString name, LH_QtSetupItem *si);

    //QString getTargetValue(int targetId);

    cf_rule_edit_mode cf_rule_editing_;
    void cf_set_edit_controls_visibility(cf_rule_edit_mode editMode = Default);

protected:
    LH_Qt_bool        *setup_cf_enabled_;

    LH_Qt_QString     *setup_cf_state_;

    LH_Qt_QTextEdit   *setup_cf_XML_;
    LH_Qt_QStringList *setup_cf_rules_;

    LH_Qt_QString     *setup_cf_new_;
    LH_Qt_QString     *setup_cf_save_;
    LH_Qt_QString     *setup_cf_cancel_;
    LH_Qt_QString     *setup_cf_delete_;
    LH_Qt_QString     *setup_cf_move_up_;
    LH_Qt_QString     *setup_cf_move_down_;
    LH_Qt_QString     *setup_cf_copy_;
    LH_Qt_QString     *setup_cf_paste_;

    void cf_source_notify(QString name, QString value, int index = 0, int count = 1);

    void add_cf_source(LH_QtSetupItem *si);
    void add_cf_source(QString name);
    void add_cf_target(LH_QtSetupItem *si);

public:
    LH_Qt_QStringList *setup_cf_source_;
    LH_Qt_QStringList *setup_cf_source_mode_;
    LH_Qt_QStringList *setup_cf_test_;
    LH_Qt_QString     *setup_cf_testValue1_;
    LH_Qt_QString     *setup_cf_testValue2_;
    LH_Qt_QStringList *setup_cf_target_;
    LH_Qt_QColor      *setup_cf_newValue_Color_;
    LH_Qt_QFont       *setup_cf_newValue_Font_;
    LH_Qt_QString     *setup_cf_newValue_String_;
    LH_Qt_QFileInfo   *setup_cf_newValue_File_;

    LH_QtCFInstance( const char *name = 0, const lh_class *cls = 0, LH_QtPlugin *parent = 0 );
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
    void cf_apply_rules(bool allowRender = true);

    void cf_copy_rules();
    void cf_paste_rules();
    void cf_move_rule_up();
    void cf_move_rule_down();
};

#endif // LH_QTCFINSTANCE_H
