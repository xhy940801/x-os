#pragma once

#include <cstdint>
#include <cstddef>

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
public:
    void init();

    void* getPersisteMemory(size_t len);
    void* getPersistePages(size_t num);

    void* allocPages(size_t level);
    void freePages(void* page, size_t level);
};
