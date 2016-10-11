#pragma once

#include <cstdint>

namespace blockbuffer
{
    const uint32_t FLAGS_UNSYNC = 0x01;
    const uint32_t FLAGS_DIRTY = 0x02;
    const uint32_t FLAGS_SYNCING = 0x04;
    const uint32_t FLAGS_FLUSHING = 0x08;
    const uint32_t FLAGS_RLOCKED = 0x10;        //could not read
    const uint32_t FLAGS_WLOCKED = 0x20;        //could not write
    const uint32_t FLAGS_LOCKED = FLAGS_RLOCKED | FLAGS_WLOCKED;
}