#ifndef LH_TICKER_H
#define LH_TICKER_H

#include <LH_QtCFInstance.h>
#include <QTimer>

class LH_Ticker : public LH_QtCFInstance
{
    Q_OBJECT

protected:
    LH_Qt_int *setup_ticks_;
    LH_Qt_int *setup_rate_;
    LH_Qt_int *setup_increment_;

public:
    LH_Ticker();

    const char *userInit();

    static lh_class *classInfo();
    QImage *render_qimage( int w, int h );

    virtual int polling();

public slots:
    void changePolling();
};

#endif // LH_TICKER_H
