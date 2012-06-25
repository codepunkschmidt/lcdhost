#ifndef EVENTLGLCDNOTIFICATION_H
#define EVENTLGLCDNOTIFICATION_H

#include "QEvent"

class EventLgLcdNotification : public QEvent
{
public:
    int code;
    int index;
    int p1;

    EventLgLcdNotification( int a_code, int i, int a_p1 = 0 ) : QEvent( type() ), code(a_code), index(i), p1(a_p1) {}
    static QEvent::Type type() { static QEvent::Type typeVal = static_cast<QEvent::Type>(registerEventType()); return typeVal; }
};

#endif // EVENTLGLCDNOTIFICATION_H
