#ifndef GOOGLETRANSLATOR_H
#define GOOGLETRANSLATOR_H

#include <QObject>
#include <QHttp>
#include <QList>
#include <QHash>
#include <QStringList>

#include "LH_QtObject.h"

#include "TranslationAPIKey.h"

#define debug_translator

enum TranslationType
{
    ttAll,
    ttMonthName,
    ttDayName,
    ttNoun
};

class IndexedPairList
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
        return names_.at(codes_.indexOf(code));
    }

    QString getCode(QString name)
    {
        return codes_.at(names_.indexOf(name));
    }
};

class GoogleTranslator: public QObject
{
    Q_OBJECT
    QString name_;
    QString targetLanguage_;
    QString sourceLanguage_;
    LH_QtObject *parent_;

    QHash<QString,QString> languageCache_;
    QHash<QString,QString> newCacheItems_;

    QHttp httpTranslate;
    QHttp httpLanguages;

    QList<QString*> translateRequestItems_;
    QList<TranslationType> translateRequestTypes_;
    QStringList translateRequestValues_;

    void apply(QStringList translatedValues);
    void apply(QString* item, QString translatedValue, TranslationType transType);

    void addToCache(QString sourceItem, QString translatedItem);

    QString getCacheFileName();
    void saveCache();
    void loadCache();

    QString fixCaps(QString src);

public:
    IndexedPairList languages;

    GoogleTranslator(QString name, LH_QtObject *parent = 0);

    void clear();
    void request();
    void addItem(QString *item, TranslationType transType = ttAll);

    void requestLanguages(QString code = "");

    QString fullDateName(QString shortName);

    bool setTargetLanguage(QString language){ if(targetLanguage_ == language) return false; else { targetLanguage_ = language; loadCache(); return true; } }
    QString targetLanguage(){ return targetLanguage_; }

signals:
    void languages_updated();

protected slots:
    void finishedTranslation(bool);
    void finishedLanguages(bool);
};

#endif // GOOGLETRANSLATOR_H
