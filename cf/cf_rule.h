#ifndef LH_QT_CF_RULE_H
#define LH_QT_CF_RULE_H

#include <QtGlobal>
#include <QList>
#include <QHash>
#include <QtXml>
#include <QObject>

#include "..\LH_Qt_bool.h"
#include "..\LH_Qt_QString.h"
#include "..\LH_Qt_QStringList.h"
#include "..\LH_Qt_QTextEdit.h"
#include "..\LH_Qt_QColor.h"
#include "..\LH_Qt_QFont.h"
#include "..\LH_Qt_QFileInfo.h"

#include "LH_QtCFInstance.h"

#ifndef EXPORT
# define EXPORT extern "C" Q_DECL_EXPORT
#endif

class cf_rule_condition: public QObject
{
protected:
    QString test_;
    QString source_;
    QString mode_;
    QStringList values_;

public:
    cf_rule_condition(QDomNode condNode, QObject* parent = 0);
    cf_rule_condition(LH_QtCFInstance *sender, QObject* parent = 0);

    QString description();
    QString test() {return test_;}
    QString source() {return source_;}
    QString mode() {return mode_;}
    QString values(int i) {
        if(i>=0 && i<values_.length())
            return values_[i];
        else
            return "";
    }

    void edit(LH_QtCFInstance* sender, cf_source_list sources)
    {
        sender->setup_cf_test_->setValue( test_ );
        sender->setup_cf_source_->setValue( source_ );
        sender->setup_cf_source_mode_->setValue( mode_ );

        sender->setup_cf_testValue1_->setFlag(LH_FLAG_HIDDEN, false);

        bool v1vis = (!sender->setup_cf_testValue1_->hasFlag(LH_FLAG_HIDDEN)) || (!sender->setup_cf_testValue1_List_->hasFlag(LH_FLAG_HIDDEN));
        LH_Qt_QStringList* source_List;

        switch((mode_=="Value"? sources[source_]->type() : lh_type_string))
        {
        case lh_type_integer_list:
        case lh_type_integer_listbox:
            sender->setup_cf_testValue1_List_->list().clear();
            source_List = (LH_Qt_QStringList*)sources[source_]->obj();

            for(int i=0; i<source_List->list().length(); i++ )
                sender->setup_cf_testValue1_List_->list().append(source_List->list().at(i));
            sender->setup_cf_testValue1_List_->refreshList();

            sender->setup_cf_testValue1_->setFlag(LH_FLAG_HIDDEN, true);
            sender->setup_cf_testValue1_List_->setFlag(LH_FLAG_HIDDEN, !v1vis);

            sender->setup_cf_testValue1_List_->setValue( values(0) );
            break;
        default:
            sender->setup_cf_testValue1_->setFlag(LH_FLAG_HIDDEN, !v1vis);
            sender->setup_cf_testValue1_List_->setFlag(LH_FLAG_HIDDEN, true);

            sender->setup_cf_testValue1_->setValue( values(0) );
            break;
        }

        sender->setup_cf_testValue2_->setValue( values(1) );
    }

    bool evaluate(cf_source_list sources, bool *ok = NULL);
    QDomNode toXmlNode(QDomDocument doc);
};

class cf_rule_action: public QObject
{
protected:
    QString type_;
public:
    cf_rule_action(QDomNode actNode, QObject* parent = 0);
    cf_rule_action(QString type, QObject* parent = 0);

    QString type() {return type_;}

    virtual QString description() { Q_ASSERT(false); return ""; }
    virtual void execute(LH_QtCFInstance* sender, cf_target_list targets) { Q_UNUSED(sender); Q_UNUSED(targets); Q_ASSERT(false); }
    virtual void edit(LH_QtCFInstance* sender, cf_target_list targets) { Q_UNUSED(sender); Q_UNUSED(targets); Q_ASSERT(false); }

    virtual bool isValid(LH_QtCFInstance* sender){ Q_UNUSED(sender); Q_ASSERT(false); return false;}

    virtual QDomNode toXmlNode(QDomDocument doc);
};

class cf_rule_action_property: public cf_rule_action
{
    QString target_;
    QString value_;

    bool setTargetValue(LH_QtCFInstance* sender, cf_target_list targets_, bool setPlaceholder = false);

    QString getRelativeFilePath(QFileInfo file, QString relativeTo)
    {
        if(!file.isFile())
            return "";
        QStringList absoluteDirectories = file.absoluteFilePath().split( '/', QString::SkipEmptyParts );
        QStringList relativeDirectories = relativeTo.split( '/', QString::SkipEmptyParts );

        //Get the shortest of the two paths
        int length = relativeDirectories.count() < absoluteDirectories.count() ? relativeDirectories.count() : absoluteDirectories.count();

        //Use to determine where in the loop we exited
        int lastCommonRoot = -1;
        int index;

        //Find common root
        for (index = 0; index < length; index++)
        if (relativeDirectories[index] == absoluteDirectories[index])
        lastCommonRoot = index;
        else
        break;

        //If we didn't find a common prefix then throw
        if (lastCommonRoot == -1)
        throw QString("Paths do not have a common base");

        //Build up the relative path
        QString relativePath;

        //Add on the ..
        for (index = lastCommonRoot + 1; index < relativeDirectories.count() - (true?1:0); index++)
        if (relativeDirectories[index].length() > 0)
        relativePath.append("../");

        //Add on the folders
        for (index = lastCommonRoot + 1; index < absoluteDirectories.count() - 1; index++)
        relativePath.append( absoluteDirectories[index] ).append( "/" );
        relativePath.append(absoluteDirectories[absoluteDirectories.count() - 1]);

        return relativePath;
    }

public:
    cf_rule_action_property(QDomNode actNode, QObject* parent = 0);
    cf_rule_action_property(LH_QtCFInstance *sender, QObject* parent = 0);

    QString description();
    QString target() {return target_;}
    QString value() {return value_;}

    void execute(LH_QtCFInstance* sender, cf_target_list targets);
    void edit(LH_QtCFInstance* sender, cf_target_list targets);

    bool isValid(LH_QtCFInstance* sender)
    {
        if(target_=="")
            return false;

        if(sender->setup_cf_target_->list().indexOf( target_ ) == -1)
            return false;

        return true;
    }
    QDomNode toXmlNode(QDomDocument doc);
};

class cf_rule : public QObject
{
public:
    cf_rule( QDomNode ruleNode, QObject *parent = 0 );
    cf_rule( LH_QtCFInstance *sender, QObject *parent = 0 );

    QString description();
    bool apply(LH_QtCFInstance* sender, cf_source_list sources_, cf_target_list targets_);

    QList<cf_rule_condition*> conditions;
    QList<cf_rule_action*> actions;

    QDomNode toXmlNode(QDomDocument doc);
};

#endif //LH_QT_CF_RULE_H
