#pragma once

#include <cstdint>

#include "List.h"
#include "wait/WaitInfo.h"

class TimeWheel
{
    List<WaitInfo, 0> dial[256];

public:
    WaitInfo* pop(uint8_t clock)
    {
        if (dial[clock].empty())
            return nullptr;
        WaitInfo* info = dial[clock].begin();
        info->removeSelf();
        return info;
    }

    WaitInfo* begin(uint8_t clock)
    {
        if (dial[clock].empty())
            return nullptr;
        return dial[clock].begin();
    }

    void push(WaitInfo* info, uint8_t clock)
    {
        dial[clock].pushBack(*info);
    }
};