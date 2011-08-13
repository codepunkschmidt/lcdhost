#ifndef EVENTLGLCDNOTIFICATION_H
#define EVENTLGLCDNOTIFICATION_H

#include "QEvent"

class EventLgLcdNotification : public QEvent
{
public:
    int code;
    int p1;
    int p2;
    int p3;
    int p4;

    EventLgLcdNotification( int a_code, int a1, int a2, int a3, int a4) :
        QEvent( type() ), code(a_code), p1(a1), p2(a2), p3(a3), p4(a4) {}
    static QEvent::Type type() { static QEvent::Type typeVal = static_cast<QEvent::Type>(registerEventType()); return typeVal; }
};

#endif // EVENTLGLCDNOTIFICATION_H
