#include <mem/SystemMemoryManager.h>

#include <common.h>

#include <mem/commondefs.h>

static inline void mapKernelMemory(uint32_t dst, uint32_t src, uint8_t type)
{
    assert(dst % 4096 == 0 && src % 4096 == 0 && type < 8);
    uint32_t* pagetable = (uint32_t*) (mem::KMEMSTART);
    pagetable[(dst - mem::KMEMSTART) >> 12] = src | type;
}

static inline void unmapKernelMemory(uint32_t dst)
{
    assert(dst % 4096 == 0);
    uint32_t* pagetable = (uint32_t*) (mem::KMEMSTART);
    pagetable[(dst - mem::KMEMSTART) >> 12] = 0;
}

void SystemMemoryManager::init()
{
    _pmm.init();
    _begin.used = reinterpret_cast<uint8_t*>(_pmm.endpos());
    size_t scap = (reinterpret_cast<size_t>(_begin.used) + mem::PAGESIZE - 1) / mem::PAGESIZE * mem::PAGESIZE;
    _begin.capacity = reinterpret_cast<uint8_t*>(scap);
    _usedEnd = reinterpret_cast<uint8_t*>(0xffea5000);
    for(uint8_t* start = _begin.capacity; start < _usedEnd; start += mem::PAGESIZE)
        unmapKernelMemory(reinterpret_cast<uint32_t>(start));
    _bm.init();
}

void* SystemMemoryManager::getPersisteMemory(size_t len)
{
    while(_begin.used + len >= _begin.capacity)
    {
        PhysicalPageInfo* info = _pmm.getOnePageUnblock();
        assert(info != nullptr);
        uint32_t addr = _pmm.getPhysicalAddress(info);
        mapKernelMemory(
            reinterpret_cast<uint32_t>(_begin.capacity),
            addr,
            3
        );
        _begin.capacity += mem::PAGESIZE;
    }
    void* rs = _begin.used;
    _begin.used += len;
    return rs;
}

void* SystemMemoryManager::getPersistePages(size_t num)
{
    for(size_t i = 0; i < num; ++i)
    {
        _usedEnd -= mem::PAGESIZE;
        PhysicalPageInfo* info = _pmm.getOnePageUnblock();
        assert(info != nullptr);
        uint32_t addr = _pmm.getPhysicalAddress(info);
        mapKernelMemory(
            reinterpret_cast<uint32_t>(_usedEnd),
            addr,
            3
        );
    }
    return _usedEnd;
}
