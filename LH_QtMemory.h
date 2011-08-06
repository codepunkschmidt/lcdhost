#ifndef LH_QTMEMORY_H
#define LH_QTMEMORY_H

#include <QObject>

#include "LH_QtInstance.h"
#include "LH_Qt_int.h"

class LH_QtMemory : public QObject
{
    Q_OBJECT

    LH_Qt_int *link_mem_phys_free_;
    LH_Qt_int *link_mem_phys_used_;
    LH_Qt_int *link_mem_phys_total_;
    LH_Qt_int *link_mem_phys_load_;
    LH_Qt_int *link_mem_virt_free_;
    LH_Qt_int *link_mem_virt_used_;
    LH_Qt_int *link_mem_virt_total_;
    LH_Qt_int *link_mem_virt_load_;

public:
    explicit LH_QtMemory( LH_QtInstance *parent );

    LH_QtInstance *parent() const { return static_cast<LH_QtInstance *>(QObject::parent()); }

    int virtLoad() { return link_mem_virt_load_->value(); }
    int physLoad() { return link_mem_phys_load_->value(); }

signals:

public slots:

};

#endif // LH_QTMEMORY_H
