
#include <stdio.h>
#include <time.h>
#include <limits.h>

#include "LH_Logger.h"

#include <QDebug>
#include <QCoreApplication>
#include <QMutex>
#include <QFileInfo>
#include <QDir>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
static QtMessageHandler lh_old_log_handler = 0;
static void lh_log_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LH_Logger::handler(type, msg);
    if(lh_old_log_handler)
        lh_old_log_handler(type, context, msg);
    return;
}
#else
#include <QDesktopServices>
static QtMsgHandler lh_old_log_handler = 0;
static void lh_log_handler(QtMsgType type, const char *msg)
{
    LH_Logger::handler(type, QString::fromLatin1(msg));
    if(lh_old_log_handler)
        lh_old_log_handler(type, msg);
    return;
}
#endif

QMutex LH_Logger::mutex_;
LH_Logger *LH_Logger::instance_ = 0;

void LH_Logger::handler(QtMsgType type, const QString &msg)
{
    mutex_.lock();
    if(instance_)
        instance_->log(type, msg);
    mutex_.unlock();
    return;
}

LH_Logger::LH_Logger(const QString &app_name, QObject *parent) :
    QObject(parent),
    app_name_(app_name)
{
    Q_ASSERT(lh_old_log_handler == 0);
    Q_ASSERT(instance_ == 0);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    QString data_dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
#else
    QString data_dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
#endif
    QDir logdir(data_dir);
    logdir.mkdir("logs");
    logdir.cd("logs");
    log_path_ = logdir.absolutePath();
    file_name_ = QString("%1/%2-%3.log")
            .arg(log_path_)
            .arg(app_name_)
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"))
            .toLocal8Bit()
            ;
    mutex_.lock();
    instance_ = this;
    mutex_.unlock();
    install();
    return;
}

LH_Logger::~LH_Logger()
{
    Q_ASSERT(instance_ == this);
    mutex_.lock();
    instance_ = 0;
    mutex_.unlock();
    uninstall();
    return;
}

void LH_Logger::install()
{
    if(lh_old_log_handler == 0)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        lh_old_log_handler = qInstallMessageHandler(lh_log_handler);
#else
        lh_old_log_handler = qInstallMsgHandler(lh_log_handler);
#endif
    }
    return;
}

void LH_Logger::uninstall()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    qInstallMessageHandler(lh_old_log_handler);
#else
    qInstallMsgHandler(lh_old_log_handler);
#endif
    lh_old_log_handler = 0;
    return;
}

void LH_Logger::cleanOldLogs()
{
    QDir dir(log_path_);
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Files | QDir::Writable))
    {
        QString name(info.fileName());
        if( name.startsWith(app_name_) && name.endsWith(".log") )
        {
            if( info.lastModified().daysTo( QDateTime::currentDateTime() ) > 10 )
                dir.remove( name );
        }
    }
    return;
}

void LH_Logger::log(QtMsgType type, const QString &msg)
{
    if(msg.isEmpty()) return;
    // if(msg.startsWith("QGLShader::link")) return;

#ifndef QT_NO_DEBUG
    if(lh_old_log_handler == 0)
    {
        uninstall();
        switch(type)
        {
        case QtDebugMsg: qDebug() << msg; break;
        case QtWarningMsg: qWarning() << msg; break;
        case QtCriticalMsg: qCritical() << msg; break;
        case QtFatalMsg: qCritical() << msg; break;
        }
        install();
    }
#endif

    if(parent())
        QCoreApplication::postEvent(parent(), new LH_LogMessage(type, msg));

    time_t t;
    time(&t);
    if(struct tm *lt = localtime(&t))
    {
        if(FILE *f = fopen(file_name_.constData(), "a"))
        {
            const char *msg_type;
            const QByteArray msg_text(msg.toLocal8Bit());

            switch( type )
            {
            case QtDebugMsg:    msg_type = "D.."; break;
            case QtWarningMsg:  msg_type = ".W."; break;
            case QtCriticalMsg: msg_type = "..E"; break;
            default:            msg_type = "***"; break;
            }

            fprintf(f, "%04d-%02d-%02d %02d:%02d:%02d | %s | %s\n",
                    lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
                    lt->tm_hour, lt->tm_min, lt->tm_sec,
                    msg_type, msg_text.constData());
#ifndef QT_NO_DEBUG
            fflush(f);
#endif
            fclose(f);
        }
    }

    return;
}

#ifdef Q_OS_WIN32
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
