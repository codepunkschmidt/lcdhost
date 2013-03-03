
#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "LH_Logger.h"

#include <QtGlobal>
#include <QDebug>
#include <QCoreApplication>
#include <QMutex>
#include <QFileInfo>
#include <QString>
#include <QDir>

#ifdef Q_OS_WIN
#include <Windows.h>
QString Win32Message( int dw )
{
    QString s;
    LPVOID lpMsgBuf;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        (DWORD) dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    s = QString::fromUtf16( (const ushort*) lpMsgBuf ).trimmed();
    LocalFree(lpMsgBuf);
    return s;
}
#endif

QMutex LH_Logger::mutex_;
LH_Logger *LH_Logger::instance_ = 0;
static lh_log_handler_t lh_log_old_handler = 0;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
static lh_log_handler_t (*lh_log_install_message_handler)(lh_log_handler_t) = qInstallMessageHandler;

QString lh_data_dir()
{
    QString data_dir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if(data_dir.isEmpty())
        return QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    return data_dir.append(QLatin1String("/LCDHost"));
}

static void lh_log_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#ifdef Q_OS_WIN
# ifdef _MSC_VER
    OutputDebugStringW((LPCWSTR) (const void*) msg.utf16());
# endif
#endif
    if(lh_log_old_handler)
        lh_log_old_handler(type, context, msg);
    if(LH_Logger *logger = LH_Logger::lock())
    {
        logger->log(type, msg);
        logger->unlock();
    }
    return;
}
#else
#include <QDesktopServices>
static lh_log_handler_t (*lh_log_install_message_handler)(lh_log_handler_t) = qInstallMsgHandler;

QString lh_data_dir()
{
    QString data_dir(QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
    if(data_dir.isEmpty())
        return QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    return data_dir.append(QLatin1String("/LCDHost"));
}
static void lh_log_handler(QtMsgType type, const char *msg_p)
{
    if(lh_log_old_handler)
        lh_log_old_handler(type, msg_p);
#ifndef QT_NO_DEBUG
    else
    {
        // Need this to get log output in Qt Creator with Qt 4
        qInstallMsgHandler(0);
        switch(type)
        {
        case QtDebugMsg:
            qDebug("%s", msg_p);
            break;
        case QtWarningMsg:
            qWarning("%s", msg_p);
            break;
        default:
            qCritical("%s", msg_p);
            break;
        }
        qInstallMsgHandler(lh_log_handler);
    }
#endif
    if(LH_Logger *logger = LH_Logger::lock())
    {
        logger->log(type, QString::fromLatin1(msg_p));
        logger->unlock();
    }
    return;
}
#endif

#if 0
static void lh_log_install(LH_Logger *logger)
{
    lh_log_mutex.lock();
    if(! lh_log_list.contains(logger))
    {
        lh_log_list.append(logger);
        if(lh_log_list.size() == 1)
            lh_log_old_handler = lh_log_install_message_handler(lh_log_handler);
    }
    lh_log_mutex.unlock();
}

static void lh_log_uninstall(LH_Logger *logger)
{
    lh_log_mutex.lock();
    if(lh_log_list.removeOne(logger))
    {
        if(lh_log_list.isEmpty())
        {
            lh_log_install_message_handler(lh_log_old_handler);
            lh_log_old_handler = 0;
        }
    }
    lh_log_mutex.unlock();
}
#endif

QString lh_log_dir()
{
    return lh_data_dir().append(QLatin1String("/logs"));
}

LH_Logger *LH_Logger::lock()
{
    if(mutex_.tryLock(1000))
    {
        if(instance_)
            return instance_;
        mutex_.unlock();
    }
    else
    {
        lh_log_install_message_handler(lh_log_old_handler);
        qCritical("LH_Logger::lock() failed to aquire mutex");
        lh_log_old_handler = lh_log_install_message_handler(lh_log_handler);
    }
    return 0;
}

LH_Logger::LH_Logger(const QString &app_name, QObject *parent) :
    QObject(parent),
    filter_count_(0)
{
    Q_ASSERT(instance_ == 0);
    Q_ASSERT(lh_log_old_handler == 0);
    mutex_.lock();
    app_name_ = app_name.isEmpty() ? QCoreApplication::applicationName() : app_name;
    QDir logdir(lh_log_dir());
    log_path_ = logdir.absolutePath();
    logdir.mkpath(log_path_);
    file_name_ = QString("%1/%2-%3.log")
            .arg(log_path_)
            .arg(app_name_)
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"))
            .toLocal8Bit()
            ;
    instance_ = this;
    mutex_.unlock();
    lh_log_old_handler = lh_log_install_message_handler(lh_log_handler);
    return;
}

LH_Logger::~LH_Logger()
{
    Q_ASSERT(instance_ == this);
    lh_log_handler_t old_handler = lh_log_old_handler;
    lh_log_old_handler = 0;
    lh_log_install_message_handler(old_handler);
    mutex_.lock();
    instance_ = 0;
    mutex_.unlock();
    qDebug("LH_Logger: %d filtered, %d known", filter_count_, last_seen_.size());
    return;
}

QString LH_Logger::fileName() const
{
    return QString::fromLocal8Bit(file_name_);
}

void LH_Logger::removeOld(int days_old) const
{
    QDir dir(log_path_);
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Files | QDir::Writable))
    {
        QString name(info.fileName());
        if( name.startsWith(app_name_) && name.endsWith(".log") )
        {
            if( info.lastModified().daysTo( QDateTime::currentDateTime() ) > days_old )
                dir.remove( name );
        }
    }
    return;
}

void LH_Logger::log(QtMsgType type, const QString &msg)
{
    if(msg.isEmpty())
        return;

    time_t t;
    time(&t);

    if(type == QtWarningMsg)
    {
        uint v = (uint) t;
        if(last_seen_.value(msg) + 30 > v)
        {
            ++ filter_count_;
            return;
        }
        last_seen_[msg] = v;
    }

    if(struct tm *lt = localtime(&t))
    {
        if(FILE *f = fopen(file_name_.constData(), "a"))
        {
            const char *msg_type = "???";
            const QByteArray msg_text(msg.toLocal8Bit());

            switch(type)
            {
            case QtDebugMsg:    msg_type = "D.."; break;
            case QtWarningMsg:  msg_type = ".W."; break;
            case QtCriticalMsg: msg_type = "..E"; break;
            case QtFatalMsg:    msg_type = "***"; break;
            }

            fprintf(f, "%04d-%02d-%02d %02d:%02d:%02d | %s | %s\n",
                    lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                    lt->tm_hour, lt->tm_min, lt->tm_sec,
                    msg_type, msg_text.constData());
#ifdef QT_NO_DEBUG
            if(type == QtCriticalMsg)
#endif
                fflush(f);
            fclose(f);
        }
    }

    emit logged(t, (int) type, msg);

    return;
}

void LH_Logger::unlock() const
{
    mutex_.unlock();
}

#if 0
QEvent::Type LH_LogMessage::type()
{
    static Type event_type = (Type) QEvent::registerEventType();
    return event_type;
}

LH_LogMessage::LH_LogMessage(QtMsgType severity, const QString &message) :
    QEvent(type()),
    msgtime(QDateTime::currentDateTime()),
    msgtype(severity),
    msgtext(message)
{
    return;
}
#endif

