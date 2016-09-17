#pragma once

#include <cstdint>

#include "sched/ScheduleQueue.h"
#include "sched/ScheduleInfo.h"

#include "task/TaskStatus.h"

class ScheduleManager
{
    ScheduleQueue queue[2];
    ScheduleQueue* active;
    ScheduleQueue* expire;
    uint16_t scheduleLoop;
    long jiffies;

public:
    ScheduleManager();

    void schedule(bool passive = false);

    void putInQueue(ScheduleInfo* info);
    
    long curJiffies()
    {
        return jiffies;
    }

    void addJiffies()
    {
        ++jiffies;
    }
};