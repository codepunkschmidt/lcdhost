#ifndef LOGITECHG19THREAD_H
#define LOGITECHG19THREAD_H

#include <QThread>
#include <QSemaphore>
#include <QImage>

#include "LogitechG19.h"

class LogitechG19Thread : public QThread
{
    Q_OBJECT

    LogitechG19 * g19_;
    volatile bool time_to_die_;
    QSemaphore sem_;
    QImage img_;

public:
    explicit LogitechG19Thread( QObject *parent = 0 );

    void run();

    LogitechG19 *device() const { return g19_; }
    void timeToDie() { time_to_die_ = true; }
    bool gotDevice() const { return g19_ != NULL; }
    bool waitToDie(int secs);
};

#endif // LOGITECHG19THREAD_H
