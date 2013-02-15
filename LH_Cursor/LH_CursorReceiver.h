#ifndef LH_CURSORRECEIVER_H
#define LH_CURSORRECEIVER_H

#include "LH_QtInstance.h"
#include "LH_Qt_QString.h"
#include "LH_Qt_QStringList.h"

#include "LH_CursorData.h"
#include <QDebug>

class LH_CursorReceiver: public QObject
{
    Q_OBJECT

public:
    LH_Qt_QString *setup_coordinate_;
    LH_Qt_QStringList *setup_cursor_state_;
    LH_Qt_QString *setup_json_data_;
    LH_Qt_QString *setup_json_postback_;

    LH_CursorReceiver(LH_QtInstance *parent, const char *amember = NULL);

    cursorData data();

    void postback(cursorData);

public slots:
    bool updateState();

signals:
    void stateChanged(bool,bool);
};

#endif // LH_CURSORRECEIVER_H

