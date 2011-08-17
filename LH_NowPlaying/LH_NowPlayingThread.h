#ifndef LH_NOWPLAYINGTHREAD_H
#define LH_NOWPLAYINGTHREAD_H

#include <QThread>

class LH_NowPlayingThread : public QThread
{
    Q_OBJECT

public:
    explicit LH_NowPlayingThread(QObject *parent = 0) : QThread((QObject*)parent) {}
    void run();
};

#endif // LH_NOWPLAYINGTHREAD_H
