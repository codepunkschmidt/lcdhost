#ifndef EVENTLGLCDBUTTON_H
#define EVENTLGLCDBUTTON_H

#include "QEvent"

class EventLgLcdButton : public QEvent
{
public:
    bool bw;
    unsigned long buttons;

    EventLgLcdButton( bool b, unsigned long ul ) : QEvent( type() ), bw(b), buttons(ul) {}
    static QEvent::Type type() { static QEvent::Type typeVal = static_cast<QEvent::Type>(registerEventType()); return typeVal; }
};


#endif // EVENTLGLCDBUTTON_H
