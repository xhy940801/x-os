#pragma once

#include "mem/PhysicalPageInfo.h"
#include "mem/commondefs.h"

class PhysicalMemoryManager
{
    PhysicalPageInfo* _memoryhead;
    PhysicalPageInfo* _fmemoryhead;
public:
    void init();
    size_t endpos();    //return the end pos of the array memoryhead
    PhysicalPageInfo* getOnePageUnblock();
    uintptr_t getPhysicalAddress(PhysicalPageInfo* pageinfo)
    {
        return (pageinfo - _memoryhead) * mem::PAGESIZE;
    }
    size_t freePageCount();
};
