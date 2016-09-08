#pragma once

#include "sched/commondefs.h"

#include <cstddef>

#include "List.h"
#include "common.h"

class ScheduleInfo;

class ScheduleQueue
{
    List<ScheduleInfo, 0> levels[sched::LEVEL_SIZE];

public:
    ScheduleQueue() = default;

    bool empty(size_t i)
    {
        assert(i < sched::LEVEL_SIZE);
        return levels[i].empty();
    }

    ScheduleInfo* pop();

    void push(ScheduleInfo* info);
};