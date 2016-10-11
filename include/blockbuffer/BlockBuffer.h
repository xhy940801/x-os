#pragma once

#include <cstddef>

#include "Map.h"
#include "List.h"

#include "blockbuffer/BlockDriver.h"
#include "blockbuffer/commondefs.h"

class BlockBuffer : public MapNode<0>, public ListNode<BlockBuffer, 0>
{
    BlockDriver* driver;
    size_t blockNO;
    uint8_t* buffer;
    uint32_t flags;

    size_t readRefCount;
    size_t writeRefCount;
};