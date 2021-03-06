#include "mem/SystemMemoryManager.h"

#include "common.h"

#include "mem/commondefs.h"
#include "mem.h"

static inline void mapKernelMemory(uint32_t dst, uint32_t src, uint8_t type)
{
    assert(dst % mem::PAGESIZE == 0 && src % mem::PAGESIZE == 0 && type < 8);
    uint32_t* pagetable = (uint32_t*) (mem::KMEMSTART);
    pagetable[(dst - mem::KMEMSTART) >> 12] = src | type;
}

static inline void unmapKernelMemory(uint32_t dst)
{
    assert(dst % mem::PAGESIZE == 0);
    uint32_t* pagetable = (uint32_t*) (mem::KMEMSTART);
    pagetable[(dst - mem::KMEMSTART) >> 12] = 0;
}

static inline uint32_t getPhysicalAddrFromVirtualAddr(uint32_t dst)
{
    uint32_t* page_table = (uint32_t*) (mem::KMEMSTART);
    return page_table[(dst - mem::KMEMSTART) >> 12] & 0xfffff000;
}

void SystemMemoryManager::init()
{
    _begin.used = reinterpret_cast<uint8_t*>(physicalMemoryManager.endpos());
    size_t scap = (reinterpret_cast<size_t>(_begin.used) + mem::PAGESIZE - 1) / mem::PAGESIZE * mem::PAGESIZE;
    _begin.capacity = reinterpret_cast<uint8_t*>(scap);
    _usedEnd = reinterpret_cast<uint8_t*>(0xffea5000);
    for(uint8_t* start = _begin.capacity; start < _usedEnd; start += mem::PAGESIZE)
        unmapKernelMemory(reinterpret_cast<uint32_t>(start));
}

void* SystemMemoryManager::getPersisteMemory(size_t len)
{
    while(_begin.used + len >= _begin.capacity)
    {
        PhysicalPageInfo* info = physicalMemoryManager.getOnePageUnblock();
        assert(info != nullptr);
        uint32_t addr = physicalMemoryManager.getPhysicalAddress(info);
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
    for (size_t i = 0; i < num; ++i)
    {
        _usedEnd -= mem::PAGESIZE;
        PhysicalPageInfo* info = physicalMemoryManager.getOnePageUnblock();
        assert(info != nullptr);
        uint32_t addr = physicalMemoryManager.getPhysicalAddress(info);
        mapKernelMemory(
            reinterpret_cast<uint32_t>(_usedEnd),
            addr,
            3
        );
    }
    return _usedEnd;
}

void* SystemMemoryManager::getOnePersistePage(uintptr_t& physicalAddr)
{
    _usedEnd -= mem::PAGESIZE;
    PhysicalPageInfo* info = physicalMemoryManager.getOnePageUnblock();
    assert(info != nullptr);
    physicalAddr = physicalMemoryManager.getPhysicalAddress(info);
    mapKernelMemory(
        reinterpret_cast<uint32_t>(_usedEnd),
        physicalAddr,
        3
    );
    return _usedEnd;
}

void* SystemMemoryManager::allocPages(size_t level)
{
    void* p = buddyManager.getAddress(level);
    size_t pcount = 1 << level;

    if (sem.down(pcount) != WakeupRet::NORMAL)
        return nullptr;

    uintptr_t cp = reinterpret_cast<uintptr_t>(p);
    for (size_t i = 0; i < pcount; ++i)
    {
        PhysicalPageInfo* info = physicalMemoryManager.getOnePageUnblock();
        mapKernelMemory(cp, physicalMemoryManager.getPhysicalAddress(info), 7);
        info->setPage(reinterpret_cast<char*>(cp));
        cp += mem::PAGESIZE;
    }
    return p;
}

void* SystemMemoryManager::allocOnePage(uintptr_t& physicalAddr)
{
    if (sem.down(1) != WakeupRet::NORMAL)
        return nullptr;

    void* p = buddyManager.getAddress(0);
    PhysicalPageInfo* info = physicalMemoryManager.getOnePageUnblock();
    physicalAddr = physicalMemoryManager.getPhysicalAddress(info);
    mapKernelMemory(reinterpret_cast<uint32_t>(p), physicalAddr, 7);
    info->setPage(reinterpret_cast<char*>(p));
    return p;
}

void SystemMemoryManager::freePages(void* page, size_t level)
{

}

void SystemMemoryManager::resetSemaphore()
{
    sem.up(physicalMemoryManager.freePageCount());
    printk("freePageCount %u\n", sem.resourceCount());
}

uintptr_t SystemMemoryManager::getPhysicalAddr(const void* ptr)
{
    return getPhysicalAddrFromVirtualAddr(reinterpret_cast<uint32_t>(ptr));
}