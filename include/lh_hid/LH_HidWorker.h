#ifndef LH_HIDWORKER_H
#define LH_HIDWORKER_H

#include <QObject>
#include <QAtomicPointer>
#include <QTime>

class QEvent;
class QTimerEvent;
class LH_HidThread;
class LH_HidDevice;
class LH_HidRequest;

class LH_HidWorker : public QObject
{
    Q_OBJECT

    static QAtomicPointer<LH_HidWorker> instance_;
    static int event_publish_;

    int timer_id_;
    bool hidapi_inited_;
    QTime last_event_time_;

public:
    static bool subscribe(QObject *recipient, const char *slot);
    virtual ~LH_HidWorker();

signals:
    void publishStart() const;
    void publish(LH_HidDevice *hd) const;
    void onlineChanged(LH_HidDevice *hd, bool state) const;

protected:
    friend class LH_HidThread;
    explicit LH_HidWorker();
    void timerEvent(QTimerEvent *ev); // enumerates HID devices
    void enumerateDevices();
    void setOnline(LH_HidDevice *hd, bool state) const;

    void customEvent(QEvent *ev);
    void processRequest(LH_HidRequest *hidreq);
    bool checkOnline(LH_HidDevice *hd) const;
    void closeDevice(LH_HidDevice *hd) const;
    void stopHidApi();
};

#endif // LH_HIDWORKER_H
