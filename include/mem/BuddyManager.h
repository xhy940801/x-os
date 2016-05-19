#pragma once

#include <cstddef>

#include <mem/BuddySlot.h>
#include <mem/commondefs.h>

class BuddyManager
{
    BuddySlot _slots[mem::BUDDYDEP];

    int getPos(size_t level);
    void freePos(int pos, size_t level);

public:
    void init();
    void* getAddress(size_t level);
    void freeAddress(void* p, size_t level);
};
