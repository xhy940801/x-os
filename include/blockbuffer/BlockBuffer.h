#pragma once

#include <cstddef>

#include "Map.h"
#include "List.h"

#include "blockbuffer/BlockDriver.h"

class BlockBuffer : public MapNode<0>, public ListNode<BlockBuffer, 0>
{
    BlockDriver* driver;
    size_t blockNO;
    uint8_t* buffer;
};