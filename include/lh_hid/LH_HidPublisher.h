#ifndef LH_HIDPUBLISHER_H
#define LH_HIDPUBLISHER_H

#include <QObject>

class LH_HidDevice;

class LH_HidPublisher : public QObject
{
    Q_OBJECT
    int publishing_;

public:
    explicit LH_HidPublisher(QObject *parent) :
        QObject(parent),
        publishing_(0)
    {}

signals:
    void onlineChanged(LH_HidDevice *hd, bool state) const;

public slots:
    void publishStart();
    void publish(LH_HidDevice *hd);
};

#endif // LH_HIDPUBLISHER_H
