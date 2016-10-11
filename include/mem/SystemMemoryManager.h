#pragma once

#include <cstdint>
#include <cstddef>

#include "multiproc.h"
#include "multiproc/MultiProcInfo.h"

class SystemMemoryManager
{
    struct MemoryUsedInfo
    {
        uint8_t* used;
        uint8_t* capacity;
    };

private:
    MemoryUsedInfo _begin;
    uint8_t* _usedEnd;
    Semaphore sem;

public:
    void init();

    void* getPersisteMemory(size_t len);
    void* getPersistePages(size_t num);
    void* getOnePersistePage(uintptr_t& physicalAddr);

    void* allocPages(size_t level);
    void* allocOnePage(uintptr_t& physicalAddr);
    void freePages(void* page, size_t level);

    void resetSemaphore();
    uintptr_t getPhysicalAddr(const void* ptr);
};
