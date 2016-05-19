#pragma once

#include <cstddef>

namespace mem
{
    const size_t PAGESIZE = 4096;
    const size_t KMEMSTART = 0xc0000000;
    const size_t USHRTMAX = 16;
    const size_t BUDDYDEP = 15;
    const size_t M1G = 1024 * 1024 * 1024;
    const size_t M1M = 1024 * 1024;
    const size_t M1K = 1024;
}
