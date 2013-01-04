#include <QtGlobal>
#include <QObject>
#include <QMutex>
#include <QEvent>
#include <QDateTime>
#include <QString>

class LH_Logger : public QObject
{
    Q_OBJECT

public:
    static LH_Logger *instance() { return instance_; }
    static void handler(QtMsgType type, const QString &msg);

    LH_Logger(const QString &app_name, QObject *parent = 0);
    virtual ~LH_Logger();

    void install();
    void cleanOldLogs();
    void log(QtMsgType type, const QString &msg);
    void uninstall();
    QString logFile() const { return QString::fromLocal8Bit(file_name_); }

protected:
    static QMutex mutex_;
    static LH_Logger *instance_;

    QString app_name_;
    QString log_path_;
    QByteArray file_name_;
};

class LH_LogMessage : public QEvent
{
public:
    static Type type()
    {
        static Type event_type = (Type) QEvent::registerEventType();
        return event_type;
    }

    QDateTime msgtime;
    QtMsgType msgtype;
    QString msgtext;

    LH_LogMessage(QtMsgType severity, const QString &message) :
        QEvent(type()),
        msgtime(QDateTime::currentDateTime()),
        msgtype(severity),
        msgtext(message)
    {}
};

#if defined(Q_OS_WIN32)
QString Win32Message( int dw );
#endif
