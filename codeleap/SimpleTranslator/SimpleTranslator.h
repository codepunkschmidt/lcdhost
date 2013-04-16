#ifndef SIMPLETRANSLATOR_H
#define SIMPLETRANSLATOR_H

#include <QObject>

#include <QList>
#include <QHash>
#include <QStringList>

#include "LH_QtObject.h"

#ifdef LH_SIMPLETRANSLATOR_LIBRARY
# define LH_SIMPLETRANSLATOR_EXPORT Q_DECL_EXPORT
#else
# define LH_SIMPLETRANSLATOR_EXPORT Q_DECL_IMPORT
#endif

enum TranslationType
{
    ttAll,
    ttMonthName,
    ttDayName,
    ttNoun
};

class LH_SIMPLETRANSLATOR_EXPORT IndexedPairList
{
    QStringList codes_;
    QStringList names_;

public:

    bool append(QString code, QString name)
    {
        if(codes_.contains(code))
            return false;
        if(names_.contains(name))
            return false;

        codes_.append(code);
        names_.append(name);
        return true;
    }

    QStringList codes()
    {
        return QStringList(codes_);
    }

    QStringList names()
    {
        return QStringList(names_);
    }

    void clear()
    {
        codes_.clear();
        names_.clear();
    }

    QString getName(QString code)
    {
        if(codes_.contains(code))
            return names_.at(codes_.indexOf(code));
        else
            return "";
    }

    QString getCode(QString name)
    {
        if(names_.contains(name))
            return codes_.at(names_.indexOf(name));
        else
            return "";
    }

    bool containsName(QString name)
    {
        return names_.contains(name);
    }

    bool containsCode(QString code)
    {
        return codes_.contains(code);
    }
};

class LH_SIMPLETRANSLATOR_EXPORT SimpleTranslator: public QObject
{
    Q_OBJECT
    bool loaded_;
    QString name_;
    QString targetLanguage_;
    QString sourceLanguage_;
    LH_QtObject *parent_;

    QHash<QString,QString> languageCache_;
    QHash<QString,QString> newCacheItems_;

    void apply(QString* item, QString translatedValue, TranslationType transType);

    void addToCache(QString sourceItem, QString translatedItem);

    QString getCacheFileName(bool externalPath = true, QString targetLanguage = "");
    void loadCache(bool languagesList = false, bool includeUntranslatedLanguages = false);

    QString fixCaps(QString src);

public:
    IndexedPairList languages;

    SimpleTranslator(QString name, LH_QtObject *parent = 0);
    void loadLanguages(bool includeUntranslatedLanguages = false) { loadCache(true, includeUntranslatedLanguages); }

    void saveCache();
    void addItem(QString *item, TranslationType transType = ttAll);

    QString fullDateName(QString shortName);

    bool setTargetLanguage(QString language){
        if(targetLanguage_ == language && loaded_)
            return false;
        else
        {
            loaded_ = true;
            targetLanguage_ = language;
            loadCache();
            return true;
        }
    }
    QString targetLanguage(){ return targetLanguage_; }
};

#endif // SIMPLETRANSLATOR_H
