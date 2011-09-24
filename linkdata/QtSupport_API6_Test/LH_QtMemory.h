#ifndef LH_QTMEMORY_H
#define LH_QTMEMORY_H

#include <QObject>

#include "LH_QtInstance.h"
#include "LH_QtSetupItem.h"

class LH_QtMemory : public QObject
{
    Q_OBJECT

    LH_QtSetupItem link_mem_phys_;
    LH_QtSetupItem link_mem_virt_;

public:
    explicit LH_QtMemory( LH_QtInstance *parent );

    LH_QtInstance *parent() const { return static_cast<LH_QtInstance *>(QObject::parent()); }

    int virtLoad();
    int physLoad();

signals:

public slots:

};

#endif // LH_QTMEMORY_H
