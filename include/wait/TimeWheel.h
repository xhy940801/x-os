#pragma once

#include <cstdint>

#include "wait/WaitInfo.h"
#include "task/TaskStatus.h"

class TimeWheel
{
    TaskStatusList dials[256];

public:
    TaskStatusInfo* pop(uint8_t clock)
    {
        if (dials[clock].empty())
            return nullptr;
        TaskStatusInfo* info = dials[clock].begin();
        info->removeSelf();
        return info;
    }

    TaskStatusInfo* begin(uint8_t clock)
    {
        if (dials[clock].empty())
            return nullptr;
        return dials[clock].begin();
    }

    void push(TaskStatusInfo* info, uint8_t clock)
    {
        dials[clock].pushBack(*info);
    }

    TaskStatusList& dial(uint8_t clock)
    {
        return dials[clock];
    }
};