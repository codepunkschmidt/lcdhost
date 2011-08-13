#ifndef EVENTLGLCDBUTTON_H
#define EVENTLGLCDBUTTON_H

#include <QEvent>

class EventLgLcdButton : public QEvent
{
public:
    unsigned long buttons;

    EventLgLcdButton( unsigned long ul ) : QEvent( type() ), buttons(ul) {}
    static QEvent::Type type() { static QEvent::Type typeVal = static_cast<QEvent::Type>(registerEventType()); return typeVal; }
};


#endif // EVENTLGLCDBUTTON_H
