#include "mem/BuddySlot.h"

#include <new>

#include "common.h"

#include "mem.h"

void BuddySlot::init(size_t listSize)
{
    void* nodesMem = memoryManager.getPersisteMemory(listSize * sizeof(Node));
    _nodes = reinterpret_cast<Node*>(nodesMem);
    new (&_list) List<Node, 0>;
    void* mapMem = memoryManager.getPersisteMemory((listSize / 16 + 3) / 4 * 4);
    memset(mapMem, 0, (listSize / 16 + 3) / 4 * 4);
    _map.setMap(reinterpret_cast<uint32_t*>(mapMem));
}
