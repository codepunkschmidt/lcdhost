
#include <QtGlobal>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <QHash>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
typedef QtMessageHandler lh_log_handler_t;
#else
typedef QtMsgHandler lh_log_handler_t;
#endif

class LH_Logger : public QObject
{
    Q_OBJECT

public:
    static LH_Logger *lock();

    LH_Logger(const QString &app_name = QString(), QObject *parent = 0);
    virtual ~LH_Logger();

    QString fileName() const;
    void removeOld(int days_old = 30) const;
    void log(QtMsgType type, const QString &msg);
    void unlock() const;

signals:
    void logged(uint msgtime, int msgtype, const QString &msgtext) const;

protected:
    static QMutex mutex_;
    static LH_Logger *instance_;

    QString app_name_;
    QString log_path_;
    QByteArray file_name_;
    QHash<QString, uint> last_seen_;
    int filter_count_;
};

QString lh_data_dir();
QString lh_log_dir();

#if defined(Q_OS_WIN32)
QString Win32Message( int dw );
#endif
