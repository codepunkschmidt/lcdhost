#ifndef EVENTWEBKITHEARTBEAT_H
#define EVENTWEBKITHEARTBEAT_H

#include <QEvent>

class EventWebKitHeartbeat : public QEvent
{
public:
    EventWebKitHeartbeat() : QEvent( type() ) {}
    static QEvent::Type type() { static QEvent::Type typeVal = static_cast<QEvent::Type>(registerEventType()); return typeVal; }
};

#endif // EVENTWEBKITHEARTBEAT_H
