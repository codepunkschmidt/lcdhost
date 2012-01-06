#include "GoogleTranslator.h"

#include <QDebug>
#include <QRegExp>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QNetworkProxyQuery>
#include <QUrl>

GoogleTranslator::GoogleTranslator(QString name, LH_QtObject *parent) : QObject(parent)
{
    parent_ = parent;
    name_ = name;
    sourceLanguage_ = "en";
    targetLanguage_ = "en";

    repLanguages = NULL;

    connect(&namLanguages, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedLanguages(QNetworkReply*)));
}

void GoogleTranslator::clear()
{
    if(repLanguages!=NULL && repLanguages->isRunning()) {
        repLanguages->abort();
        repLanguages->deleteLater();
        repLanguages = NULL;
    }

    translateRequestItems_.clear();
    translateRequestTypes_.clear();
    translateRequestValues_.clear();
}

void GoogleTranslator::addItem(QString *item, TranslationType transType)
{
    if((*item).trimmed() == "" || (*item).trimmed() == "N/A")
        return;

    QString value = *item;
    QString rxString = "";
    switch(transType)
    {
    case ttMonthName:
        rxString = "(Jan(?:uary)?|Feb(?:ruary)?|Mar(?:ch)?|Apr(?:il)?|May|Jun(?:e)?|Jul(?:y)?|Aug(?:ust)?|Sep(?:tember)?|Oct(?:ober)?|Nov(?:ember)?|Dec(?:ember)?)";
        break;
    case ttDayName:
        rxString = "(Mon(?:day)?|Tue(?:sday)?|Wed(?:nesday)?|Thu(?:rsday)?|Fri(?:day)?|Sat(?:urday)?|Sun(?:day)?)";
        break;
    default:
        break;
    }
    if(rxString!="")
    {
        QRegExp rx(rxString);
        if(rx.indexIn(value)!=-1)
            value = fullDateName(rx.cap(1));
        else
            value = "";
    }

    if (value=="") return;

    if(transType!=ttNoun)
        value = value.toLower();

    if(languageCache_.keys().contains(value.toLower()))
        apply(item, languageCache_[value.toLower()], transType);
    else
    {
        translateRequestItems_.append(item);
        translateRequestTypes_.append(transType);
        translateRequestValues_.append(value);
    }
}

QString GoogleTranslator::fullDateName(QString shortName)
{
    QString longNames[19] = {"January","February","March","April","May","June","July","August","September","October","Novemeber","December",
                             "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"
                            };
    for(int i=0; i<19; i++)
        if(shortName == longNames[i].left(3))
            return longNames[i];

    return shortName;
}

void GoogleTranslator::apply(QStringList translatedValues)
{
    Q_ASSERT(translateRequestItems_.count() == translatedValues.count());

    for(int i=0; i<translateRequestItems_.count(); i++)
        apply(translateRequestItems_[i],translatedValues[i],translateRequestTypes_[i]);
}

void GoogleTranslator::apply(QString* item, QString translatedValue, TranslationType transType)
{
    QString value = *item;
    QString rxString = "";
    switch(transType)
    {
    case ttMonthName:
        rxString = "(Jan(?:uary)?|Feb(?:ruary)?|Mar(?:ch)?|Apr(?:il)?|May|Jun(?:e)?|Jul(?:y)?|Aug(?:ust)?|Sep(?:tember)?|Oct(?:ober)?|Nov(?:ember)?|Dec(?:ember)?)";
        break;
    case ttDayName:
        rxString = "(Mon(?:day)?|Tue(?:sday)?|Wed(?:nesday)?|Thu(?:rsday)?|Fri(?:day)?|Sat(?:urday)?|Sun(?:day)?)";
        break;
    default:
        addToCache(value, translatedValue);
        value = fixCaps(translatedValue.trimmed());
        break;
    }
    if(rxString!="")
    {
        QRegExp rx(rxString);
        if(rx.indexIn(value)!=-1)
        {
            QString translation = translatedValue.trimmed();
            translation = fixCaps(translation);
            addToCache(fullDateName(rx.cap(0)), translation);
            if(transType == ttDayName)
                translation = translation.left(3);
            value = value.replace(rx, translation);
        }
    }

    *item = value;
}

QString GoogleTranslator::fixCaps(QString src)
{
    src = src.replace(QRegExp("(^|\\W)pm(\\W|$)"),"\\1PM\\2");
    src = src.replace(QRegExp("(^|\\W)am(\\W|$)"),"\\1AM\\2");

    QRegExp rx("(?:^|\\W)([a-z])");
    int pos = 0;
    while((pos = rx.indexIn(src, pos)+1)!=0)
    {
        QString capText = rx.cap(0);
        src = QString("%1%2%3")
                .arg(pos<=1? "" : src.left(pos-1))
                .arg(capText.toUpper())
                .arg(src.length()-pos-capText.length()+1 <= 0 ? "" : src.right(src.length()-pos-capText.length()+1));
    }
    return src;
}

void GoogleTranslator::addToCache(QString sourceItem, QString translatedItem)
{
    if( languageCache_.keys().contains(sourceItem.toLower()) )
        return; //already in cache;

    languageCache_.insert(sourceItem.toLower(), translatedItem.trimmed());

    newCacheItems_.insert(sourceItem, translatedItem.trimmed());
}

QString GoogleTranslator::getCacheFileName(QString targetLanguage)
{
    if (targetLanguage == "") targetLanguage = targetLanguage_;
    return QString("%1translations.%2.%3.%4.cache")
            .arg(QString::fromUtf8(parent_->state()->dir_data))
            .arg(name_)
            .arg(sourceLanguage_)
            .arg(targetLanguage);
}

void GoogleTranslator::saveCache()
{
    QFile file(getCacheFileName());
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        foreach(QString key, newCacheItems_.keys())
            out << key << "\t" << newCacheItems_[key] << "\n";
        file.close();
        newCacheItems_.clear();
    }
}

void GoogleTranslator::loadCache()
{
    languageCache_.clear();
    QFile file(getCacheFileName());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        QStringList cachedItems = out.readAll().split("\n", QString::SkipEmptyParts);
        file.close();

        for(int i=0; i<cachedItems.length(); i++)
        {
            QStringList itemPair = cachedItems[i].split("\t");
            if(itemPair.count()==2)
                languageCache_.insert(itemPair[0].toLower(),itemPair[1]);
        }
    }
}

void GoogleTranslator::requestLanguages(QString code)
{
    const QString host = "www.googleapis.com";
    const QString path = "/language/translate/v2/languages";

    if(repLanguages!=NULL && repLanguages->isRunning()) {
        repLanguages->abort();
        repLanguages->deleteLater();
        repLanguages = NULL;
    }

    QNetworkProxyQuery npq(QUrl(QString("https://%1%2").arg(host).arg(path)));
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    if(listOfProxies.count()!=0)
    {
        if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy) {
            #ifdef debug_translator
                qDebug() << "GoogleTranslator: Using Proxy: " << listOfProxies.at(0).hostName()<< ":" << QString::number(listOfProxies.at(0).port());
            #endif
            namLanguages.setProxy(listOfProxies.at(0));
        } else {
           #ifdef debug_translator
               qDebug() << "GoogleTranslator: No proxy required.";
           #endif
        }
    } else {
        #ifdef debug_translator
            qDebug() << "GoogleTranslator: No proxy listed.";
        #endif
    }

    if(code=="")
        code = targetLanguage_;
    QString url = QString("https://%1%2?target=%3&key=%4").arg(host).arg(path).arg(code).arg(TRANSLATION_API_KEY);

#ifdef debug_translator
    qDebug() << "GoogleTranslator: Requesting Languages from: " << QString("https://%1%2").arg(host).arg(path);
#endif

    QNetworkRequest reqLanguages = QNetworkRequest(QUrl(url));
    reqLanguages.setHeader( QNetworkRequest::ContentLengthHeader, 0 );
    reqLanguages.setRawHeader("X-HTTP-Method-Override", "GET");
    repLanguages = namLanguages.get( reqLanguages );
}

void GoogleTranslator::finishedLanguages(QNetworkReply* reply)
{
    if( reply )
    {
        if( reply->error() == QNetworkReply::NoError )
        {
            if( reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) == 200 )
            {
                #ifdef debug_translator
                    qDebug() << "GoogleTranslator: HTTP (Language) Request Complete" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                #endif

                QString response = QString::fromUtf8(reply->readAll()).trimmed();

                QRegExp rxValid("^\\{\\s*\"data\":\\s*\\{\\s*\"languages\":");
                if(rxValid.indexIn(response)==-1)
                {
                    #ifdef debug_translator
                        qDebug() << "GoogleTranslator: HTTP (Language) Unrecognised Response: " << response;
                    #else
                        qDebug() << "GoogleTranslator: HTTP (Language) Unrecognised Response.";
                    #endif
                }
                else
                {
                    QRegExp rxDecode("&#([0-9]*);");
                    QRegExp rxItem("\\{\\s*"
                                   "\"language\":\\s*\"([^\"]*)\",\\s*"
                                   "\"name\":\\s*\"([^\"]*)\"\\s*"
                                   "\\}");
                    int pos=0;

                    languages.clear();

                    bool newLangs = false;
                    while((pos = rxItem.indexIn(response, pos)+1)!=0)
                    {
                        QString langCode = rxItem.cap(1);
                        while(rxDecode.indexIn(langCode)!=-1)
                            langCode = langCode.replace(rxDecode.cap(0), QString("%1").arg((char)rxDecode.cap(1).toInt()));
                        QString langName = rxItem.cap(2);
                        while(rxDecode.indexIn(langName)!=-1)
                            langName = langName.replace(rxDecode.cap(0), QString("%1").arg((char)rxDecode.cap(1).toInt()));

                        if (langCode=="en" || QFileInfo(getCacheFileName(langCode)).exists())
                            newLangs = languages.append(langCode,langName) || newLangs;
                    }
                    if(newLangs)
                        emit(languages_updated());
                }
             }
            else
                qWarning() << "GoogleTranslator: Error during HTTP (Language) fetch:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->errorString() << reply->url().toString();
        }
        else
            qWarning() << "GoogleTranslator: Error during HTTP (Language) fetch:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) << reply->errorString() << reply->url().toString();
        reply->deleteLater();
        reply = NULL;
    }
    repLanguages->deleteLater();
    repLanguages = NULL;
}
