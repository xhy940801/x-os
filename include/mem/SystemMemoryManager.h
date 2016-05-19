#pragma once

#include <mem/BuddyManager.h>
#include <mem/PhysicalMemoryManager.h>

class SystemMemoryManager
{
    struct MemoryUsedInfo
    {
        uint8_t* used;
        uint8_t* capacity;
    };

private:
    PhysicalMemoryManager _pmm;
    BuddyManager _bm;
    MemoryUsedInfo _begin;
    uint8_t* _usedEnd;
public:
    void init();

    void* getPersisteMemory(size_t len);
    void* getPersistePages(size_t num);

};
