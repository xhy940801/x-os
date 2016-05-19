#include <mem/PhysicalMemoryManager.h>

#include <new>
#include <common.h>

#include <systemparams.h>
#include <task.h>

extern char _end[];

void PhysicalMemoryManager::init()
{
    _memoryhead = reinterpret_cast<PhysicalPageInfo*>(_end);
    _fmemoryhead = _memoryhead;
    TaskInfo* process0task = taskManager.process0task();
    size_t usedpos = (reinterpret_cast<size_t>(_end) + mem::PAGESIZE - 1) / mem::PAGESIZE * mem::PAGESIZE;
    size_t pageInfoCount = systemParams.memsize() / mem::PAGESIZE;
    assert(pageInfoCount * sizeof(PhysicalPageInfo) + reinterpret_cast<size_t>(_end) < 16 * 1024 * 1024 + mem::KMEMSTART);
    //less 640k
    size_t i = 0;
    for(; i < 640 * 1024 / mem::PAGESIZE; ++i)
        new (_memoryhead + i) PhysicalPageInfo(PhysicalPageInfo::Type::NORMAL, _memoryhead + i + 1);
    size_t nlk = i - 1;
    //640k-end-align-pagesize
    for(; i < (usedpos - mem::KMEMSTART) / mem::PAGESIZE; ++i)
        new (_memoryhead + i) PhysicalPageInfo(PhysicalPageInfo::Type::KERNEL, process0task);
    size_t ntg = i;
    for(; i < pageInfoCount; ++i)
        new (_memoryhead + i) PhysicalPageInfo(PhysicalPageInfo::Type::NORMAL, _memoryhead + i + 1);
    _memoryhead[nlk]._next = _memoryhead + ntg;
    _memoryhead[i - 1]._next = nullptr;
}

size_t PhysicalMemoryManager::endpos()
{
    size_t pageInfoCount = systemParams.memsize() / mem::PAGESIZE;
    return reinterpret_cast<size_t>(_memoryhead + pageInfoCount);
}

PhysicalPageInfo* PhysicalMemoryManager::getOnePageUnblock()
{
    if(_fmemoryhead == nullptr)
        return nullptr;
    PhysicalPageInfo* rs = _fmemoryhead;
    _fmemoryhead = _fmemoryhead->_next;
    return rs;
}
