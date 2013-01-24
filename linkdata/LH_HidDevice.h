#ifndef LH_HIDDEVICE_H
#define LH_HIDDEVICE_H

#include <QObject>
#include <QByteArray>
#include <QString>

class LH_HidWorker;

class LH_HidDevice : public QObject
{
    Q_OBJECT

public:
    // slot signature must match "onlineChanged(LH_HidDevice*,bool)"
    static void subscribe(QObject *recipient, const char *slot);

    const char *device_path() const { return device_path_.constData(); }
    int vendor_id() const { return vendor_id_; }
    int product_id() const { return product_id_; }
    int device_version() const { return device_version_; }
    QString serial_text() const { return serial_text_; }
    QString vendor_text() const { return vendor_text_; }
    QString product_text() const { return product_text_; }
    int usage_page() const { return usage_page_; }
    int usage() const { return usage_; }
    const char *error() const { return error_.constData(); }
    int errorCount() const { return error_count_; }

    bool online();
    QByteArray read(int max_size = 256);
    int write(const QByteArray &report);
    QByteArray readFeature(int n, int max_size = 256);
    int writeFeature(const QByteArray &report);

signals:
    void onlineChanged(bool online);

protected:
    friend class LH_HidWorker;

    explicit LH_HidDevice(
            const QString &device_path,
            int vendor_id,
            int product_id,
            int device_version,
            const QString &serial_text,
            const QString &vendor_text,
            const QString &product_text,
            int usage_page,
            int usage,
            QObject *parent = 0);
    void setError(const QString &text);

    void *handle_;
    bool unused_;
    bool online_;
    QByteArray device_path_; // objectName() in UTF-8
    int vendor_id_;
    int product_id_;
    int device_version_;
    QString serial_text_;
    QString vendor_text_;
    QString product_text_;
    int usage_page_;
    int usage_;
    QByteArray error_; // local 8 bit
    int error_count_;
};

#endif // LH_HIDDEVICE_H
