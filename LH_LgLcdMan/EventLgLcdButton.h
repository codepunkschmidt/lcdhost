#ifndef EVENTLGLCDBUTTON_H
#define EVENTLGLCDBUTTON_H

#include "EventBase.h"

class EventLgLcdButton : public EventBase
{
public:
    bool bw;
    unsigned long buttons;

    EventLgLcdButton( bool b, unsigned long ul ) : EventBase( type() ), bw(b), buttons(ul) {}
    static QEvent::Type type() { static QEvent::Type typeVal = static_cast<QEvent::Type>(registerEventType(__FILE__)); return typeVal; }
};


#endif // EVENTLGLCDBUTTON_H
