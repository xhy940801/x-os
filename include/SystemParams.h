#pragma once

#include <cstdint>

class SystemParams
{
    uint32_t _memsize;
public:
    void initWithPtr(void* ptr);

    uint32_t memsize() const
    {
        return _memsize;
    }
};

void initSystemParamsModule();

extern SystemParams systemParams;
