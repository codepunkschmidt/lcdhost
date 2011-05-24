#include "GoogleTranslator.h"

#include <QDebug>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QNetworkProxyQuery>
#include <QUrl>

GoogleTranslator::GoogleTranslator(QString name, LH_QtObject *parent) : QObject(parent)
{
    parent_ = parent;
    name_ = name;
    sourceLanguage_ = "en";
    targetLanguage_ = "en";
    connect(&httpTranslate, SIGNAL(done(bool)), this, SLOT(finishedTranslation(bool)));
    connect(&httpLanguages, SIGNAL(done(bool)), this, SLOT(finishedLanguages(bool)));
}

void GoogleTranslator::clear()
{
    httpTranslate.abort();
    httpLanguages.abort();
    translateRequestItems_.clear();
    translateRequestTypes_.clear();
    translateRequestValues_.clear();
}

void GoogleTranslator::request()
{
    if(targetLanguage_==sourceLanguage_)
        return;

    if(translateRequestValues_.length()==0)
        return;

    const QString host = "www.googleapis.com";
    const QString path = "/language/translate/v2";

    QString params = translateRequestValues_.join("&q=");

    QNetworkProxyQuery npq(QUrl(QString("https://%1%2").arg(host).arg(path)));
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    if(listOfProxies.count()!=0)
    {
        if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy) {
            #ifdef debug_translator
                qDebug() << "GoogleTranslator: Using Proxy: " << listOfProxies.at(0).hostName()<< ":" << QString::number(listOfProxies.at(0).port());
            #endif
            httpTranslate.setProxy(listOfProxies.at(0));
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

    httpTranslate.setHost(host, QHttp::ConnectionModeHttps);

    QString url = QString("%1?source=%2&target=%3&key=%4").arg(path).arg(sourceLanguage_).arg(targetLanguage_).arg(TRANSLATION_API_KEY);
    QByteArray textByteArray("q=");
    textByteArray.append( params.toUtf8() );

    QHttpRequestHeader header = QHttpRequestHeader("POST", url, 1, 1);
    header.setContentType("application/x-www-form-urlencoded");
    header.setValue("Host", host);
    header.setValue("User-Agent", "Mozilla/5.0");
    header.setValue("Accept-Encoding", "deflate");
    header.setValue("X-HTTP-Method-Override", "GET");
    header.setContentLength(params.length());
    header.setValue("Connection", "Close");
    if(params.length()>=5000)
        qWarning() << "Not permitted to request a translation of this length";
    else
        httpTranslate.request(header,textByteArray);
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

void GoogleTranslator::finishedTranslation(bool error)
{
    if (error)
    {
        qWarning() << "GoogleTranslator: Error during HTTP (Translate) fetch:" << httpTranslate.errorString() << httpTranslate.lastResponse().statusCode();
    }
    else
    {
        #ifdef debug_translator
            qDebug() << "GoogleTranslator: HTTP (Translate) Request Complete" << httpTranslate.lastResponse().statusCode();
        #endif
        QString response = QString::fromUtf8(httpTranslate.readAll()).trimmed();

        QRegExp rxValid("^\\{\\s*\"data\":\\s*\\{\\s*\"translations\":");
        if(rxValid.indexIn(response)==-1)
        {
            #ifdef debug_translator
                qDebug() << "GoogleTranslator: HTTP (Translate) Unrecognised Response: " << response;
            #else
                qDebug() << "GoogleTranslator: HTTP (Translate) Unrecognised Response.";
            #endif
        }
        else
        {
            QRegExp rxDecode("&#([0-9]*);");
            QRegExp rxItem("\\{\\s*\"translatedText\":\\s*\"([^\"]*)\"\\s*\\}");
            QStringList translations;
            int pos=0;
            while((pos = rxItem.indexIn(response, pos)+1)!=0)
            {
                QString item = rxItem.cap(1);
                while(rxDecode.indexIn(item)!=-1)
                    item = item.replace(rxDecode.cap(0), QString("%1").arg((char)rxDecode.cap(1).toInt()));
                translations.append(item);
            }
            apply(translations);
            saveCache();
        }
    }
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

QString GoogleTranslator::getCacheFileName()
{
    const char *dir_data_p = 0;
    parent_->callback( lh_cb_dir_data, &dir_data_p );
    return QString("%1translations.%2.%3.%4.cache")
            .arg(dir_data_p)
            .arg(name_)
            .arg(sourceLanguage_)
            .arg(targetLanguage_);
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

    httpLanguages.abort();
    httpLanguages.setHost(host, QHttp::ConnectionModeHttps);

    QNetworkProxyQuery npq(QUrl(QString("https://%1%2").arg(host).arg(path)));
    QList<QNetworkProxy> listOfProxies = QNetworkProxyFactory::systemProxyForQuery(npq);
    if(listOfProxies.count()!=0)
    {
        if(listOfProxies.at(0).type() != QNetworkProxy::NoProxy) {
            #ifdef debug_translator
                qDebug() << "GoogleTranslator: Using Proxy: " << listOfProxies.at(0).hostName()<< ":" << QString::number(listOfProxies.at(0).port());
            #endif
            httpLanguages.setProxy(listOfProxies.at(0));
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
    QString url = QString("%1?target=%2&key=%3").arg(path).arg(code).arg(TRANSLATION_API_KEY);

    QHttpRequestHeader header = QHttpRequestHeader("POST", url, 1, 1);
    header.setContentType("application/x-www-form-urlencoded");
    header.setValue("Host", host);
    header.setContentLength(0);
    header.setValue("User-Agent", "Mozilla/5.0");
    header.setValue("Accept-Encoding", "deflate");
    header.setValue("X-HTTP-Method-Override", "GET");
    header.setValue("Connection", "Close");
    #ifdef debug_translator
        qDebug() << "GoogleTranslator: Requesting Languages from: " << QString("https://%1%2").arg(host).arg(path);
    #endif
    httpLanguages.request(header);
}

void GoogleTranslator::finishedLanguages(bool error)
{
    if (error)
    {
        qWarning() << "GoogleTranslator: Error during HTTP (Language) fetch:" << httpLanguages.errorString() << httpLanguages.lastResponse().statusCode();
    }
    else
    {
        #ifdef debug_translator
            qDebug() << "GoogleTranslator: HTTP (Language) Request Complete" << httpLanguages.lastResponse().statusCode();
        #endif
        QString response = QString::fromUtf8(httpLanguages.readAll()).trimmed();

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
                newLangs = languages.append(langCode,langName) || newLangs;
            }
            if(newLangs)
                emit(languages_updated());
        }
    }
}
