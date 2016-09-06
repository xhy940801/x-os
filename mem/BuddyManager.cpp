#include "mem/BuddyManager.h"

#include "common.h"

void BuddyManager::init()
{
    for(size_t i = 0, listSize = mem::M1G / mem::PAGESIZE ; i < mem::BUDDYDEP; ++i, listSize /= 2)
        _slots[i].init(listSize);
    assert(mem::PAGESIZE << (mem::BUDDYDEP - 1) == 64 * mem::M1M);
    for(size_t i = mem::M1G / (64 * mem::M1M) - 2; i > 0 ; --i)
        _slots[mem::BUDDYDEP - 1].freeNode(i);
}

int BuddyManager::getPos(size_t level)
{
    if(level == mem::BUDDYDEP)
        return -1;
    int pos = _slots[level].getNode();
    if(pos < 0)
    {
        pos = getPos(level + 1);
        if(pos < 0)
            return pos;
        pos *= 2;
        _slots[level].freeNode(pos + 1);
        assert(_slots[level].nodeState(pos));
        return pos;
    }
    return pos;
}

void BuddyManager::freePos(int pos, size_t level)
{
    assert(level != mem::BUDDYDEP);
    if(level == mem::BUDDYDEP - 1)
    {
        _slots[level].freeNode(pos);
        return;
    }
    if(_slots[level].freeNodeAndMerge(pos))
        return;
    freePos(pos / 2, level + 1);
}

void* BuddyManager::getAddress(size_t level)
{
    assert(level < mem::BUDDYDEP);
    int pos = getPos(level);
    printk("pos %d\n", pos);
    if(pos < 0)
        return nullptr;
    size_t rs = pos * (1 << level) * mem::PAGESIZE + mem::KMEMSTART;
    printk("get addr pos: %u\n", rs);
    return reinterpret_cast<void*>(rs);
}

void BuddyManager::freeAddress(void* p, size_t level)
{
    assert(level < mem::BUDDYDEP);
    size_t ptr = reinterpret_cast<size_t>(p);
    assert(ptr > mem::KMEMSTART);
    ptr -= mem::KMEMSTART;
    assert(ptr % ((1 << level) * mem::PAGESIZE) == 0);
    size_t pos = ptr / ((1 << level) * mem::PAGESIZE);
    freePos(pos, level);
}
