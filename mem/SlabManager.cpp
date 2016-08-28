#include "mem/Slabmanager.h"

#include "common.h"
#include "mem.h"

bool SlabManager::mallocOnePage(size_t level)
{
    Page* page = reinterpret_cast<Page*>(memoryManager.allocPages(0));
    if (page == nullptr)
        return false;
    page->usedcount = 0;
    size_t slabsize = (level + 1) * mem::MINSLOT;
    size_t slabcount = (mem::PAGESIZE - sizeof(Page)) / slabsize;
    Slab* slab = page->slabs;
    for (size_t i = 0; i < slabcount; ++i)
    {
        slots[level].pushFront(*slab);
        slab = nextSlab(slab, level);
    }
    assert(slabsize >= mem::MINSLOT);
    slabsize -= sizeof(Slab);
    if (slabsize < mem::MINSLOT)
    {
        assert(slabsize == 0);
        return true;
    }
    level = slabsize / mem::MINSLOT - 1;
    slots[level].pushFront(*slab);
    assert((reinterpret_cast<uintptr_t>(slab) + (level + 1) * mem::MINSLOT) % mem::PAGESIZE == 0);
    return true;
}

void* SlabManager::alloc(size_t level)
{
    if (slots[level].empty() && !mallocOnePage(level))
        return nullptr;
    Slab* slab = slots[level].begin();
    slab->removeSelf();
    Page* page = getPageOfSlab(slab);
    ++page->usedcount;
    return slab;
}

void SlabManager::free(void* buffer, size_t level)
{
    Slab* slab = reinterpret_cast<Slab*>(buffer);
    slots[level].pushFront(*slab);
    Page* page = getPageOfSlab(slab);
    assert(page->usedcount > 0);
    --page->usedcount;
    if (page->usedcount == 0)
        memoryManager.freePages(page, 0);
}