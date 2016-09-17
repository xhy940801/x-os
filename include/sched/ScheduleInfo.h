#pragma once

#include <cstdint>

#include "sched/commondefs.h"

#include "List.h"
#include "common.h"
#include "asm.h"

enum class ScheduleInfoState : uint16_t
{
    RUNNING, WAITING
};

class ScheduleInfo
{
    uint8_t nice;
    uint8_t originalNice;
    mutable uint16_t restTime;
    uint16_t scheduleLoop;

    friend class ScheduleManager;
    friend class ScheduleQueue;

protected:
    ScheduleInfo(uint8_t _nice);
    
public:
    ScheduleInfo(unsigned long options, const ScheduleInfo& scheduleInfo);

    ScheduleInfo() = delete;
    ScheduleInfo(const ScheduleInfo&) = delete;
    ScheduleInfo(ScheduleInfo&&) = delete;

    ScheduleInfo& operator = (const ScheduleInfo&) = delete;
    ScheduleInfo& operator = (ScheduleInfo&&) = delete;

public:
    uint8_t curnice() const
    {
        return nice / 16;
    }

    void resetRestTime() const
    {
        restTime = curnice() + 1;
    }

    void incNice()
    {
        uint8_t maxNice = originalNice < sched::LEVEL_SIZE - 5 ? originalNice + 5 : sched::LEVEL_SIZE - 1;
        assert(nice <= maxNice * 16 + 15);
        if (isMaxValue(nice))
            return;
        if (nice < maxNice * 16 + 15)
            ++nice;
    }

    void decNice()
    {
        uint8_t minNice = originalNice > 5 ? originalNice - 5 : 0;
        assert(nice >= minNice * 16);
        if (isMinValue(nice))
            return;
        if (nice > minNice * 16)
            --nice;
    }

};