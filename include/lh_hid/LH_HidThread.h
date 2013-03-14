#ifndef LH_HIDTHREAD_H
#define LH_HIDTHREAD_H

#include <QThread>

#if defined(LH_HID_LIBRARY)
# define LH_HID_EXPORT Q_DECL_EXPORT
#else
# define LH_HID_EXPORT Q_DECL_IMPORT
#endif

class LH_HID_EXPORT LH_HidThread : public QThread
{
    Q_OBJECT
public:
    explicit LH_HidThread(QObject *parent = 0) : QThread(parent) {}
    void run();
};

#endif // LH_HIDTHREAD_H
