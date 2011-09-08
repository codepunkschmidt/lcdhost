#include "LH_QtMemory.h"

LH_QtMemory::LH_QtMemory(LH_QtInstance *parent) : QObject(parent)
{
    link_mem_phys_free_ = new LH_Qt_int(parent,"LinkMemPhysFree",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_phys_free_->setLink("/system/memory/physical/free");
    link_mem_phys_used_ = new LH_Qt_int(parent,"LinkMemPhysUsed",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_phys_used_->setLink("/system/memory/physical/used");
    link_mem_phys_total_ = new LH_Qt_int(parent,"LinkMemPhysTotal",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_phys_total_->setLink("/system/memory/physical/total");
    link_mem_phys_load_ = new LH_Qt_int(parent,"LinkMemPhysLoad",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_phys_load_->setLink("/system/memory/physical/load");

    link_mem_virt_free_ = new LH_Qt_int(parent,"LinkMemVirtFree",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_virt_free_->setLink("/system/memory/virtual/free");
    link_mem_virt_used_ = new LH_Qt_int(parent,"LinkMemVirtUsed",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_virt_used_->setLink("/system/memory/virtual/used");
    link_mem_virt_total_ = new LH_Qt_int(parent,"LinkMemVirtTotal",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_virt_total_->setLink("/system/memory/virtual/total");
    link_mem_virt_load_ = new LH_Qt_int(parent,"LinkMemVirtLoad",0,LH_FLAG_HIDDEN|LH_FLAG_NOSAVE);
    link_mem_virt_load_->setLink("/system/memory/virtual/load");
}
