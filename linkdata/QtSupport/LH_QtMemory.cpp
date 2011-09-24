#include "LH_QtMemory.h"

LH_QtMemory::LH_QtMemory(LH_QtInstance *parent) :
    QObject(parent),
    link_mem_phys_(parent,
                   "LinkMemPhys",
                   lh::val(0,lh_link_sink),
                   lh::min(0,lh_link_sink),
                   lh::max(0,lh_link_sink),
                   lh::link("/system/memory/physical")
                   ),
    link_mem_virt_(parent,
                   "LinkMemVirt",
                   lh::val(0,lh_link_sink),
                   lh::min(0,lh_link_sink),
                   lh::max(0,lh_link_sink),
                   lh::link("/system/memory/virtual")
                   )
{
}

int LH_QtMemory::virtLoad()
{
    return link_mem_virt_.maximum().toLongLong() * 10000 / link_mem_virt_.value().toLongLong();
}

int LH_QtMemory::physLoad()
{
    return link_mem_phys_.maximum().toLongLong() * 10000 / link_mem_phys_.value().toLongLong();
}
