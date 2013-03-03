#ifndef LOGITECHG19THREAD_H
#define LOGITECHG19THREAD_H

#include <QThread>
#include <QImage>

#include "LogitechG19.h"

class LogitechG19Thread : public QThread
{
    Q_OBJECT

    volatile bool time_to_die_;
    volatile bool refresh_;

public:
    explicit LogitechG19Thread( QObject *parent = 0 );

    void run();

    void timeToDie() { time_to_die_ = true; }
    void refresh() { refresh_ = true; }
    bool waitToDie(int secs);
};

#endif // LOGITECHG19THREAD_H
