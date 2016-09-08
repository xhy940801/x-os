#pragma once

#include <cstdint>

#include "sched/ScheduleQueue.h"
#include "sched/ScheduleInfo.h"

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

    void putInQueue(ScheduleInfo* info)
    {
        if (info->restTime == 0)
        {
            info->resetRestTime();
            if (info->scheduleLoop == scheduleLoop)
                expire->push(info);
            else
                active->push(info);
        }
        else
            active->push(info);
        info->state = ScheduleInfoState::RUNNING;
    }

    void takeOutQueue(ScheduleInfo* info, ScheduleInfoState state)
    {
        assert(state != ScheduleInfoState::RUNNING);
        info->outQueue();
        info->state = state;
        info->scheduleLoop = scheduleLoop;
    }

    long curJiffies()
    {
        return jiffies;
    }

    void addJiffies()
    {
        ++jiffies;
    }
};