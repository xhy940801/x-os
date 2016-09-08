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

class ScheduleInfo : public ListNode<ScheduleInfo, 0>
{
public:
    class Locker
    {
        ScheduleInfo* info;
        Locker(ScheduleInfo* _info) : info(_info)
        {
            assert(info != nullptr);
            info->lock();
        }

        friend class ScheduleInfo;

    public:
        Locker() = delete;
        Locker(const Locker&) = delete;
        Locker& operator = (const Locker&) = delete;
        Locker& operator = (Locker&&) = delete;

        Locker(Locker&& locker) : info(locker.info)
        {
            locker.info = nullptr;
        }

        ~Locker()
        {
            if (info != nullptr)
                info->unlock();
        }
    };

private:
    uint8_t nice;
    uint8_t originalNice;
    ScheduleInfoState state;
    mutable uint16_t restTime;
    uint16_t scheduleLoop;
public:
    uint32_t lockCount;

    friend class ScheduleManager;

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

    void outQueue()
    {
        ListNode<ScheduleInfo, 0>::removeSelf();
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

    void lock()
    {
        if (lockCount == 0)
            _cli();
        ++lockCount;
    }

    void unlock()
    {
        assert(lockCount > 0);
        if (lockCount == 1)
            _sti();
        --lockCount;
    }

    void vLock()
    {
        ++lockCount;
    }

    void vUnlock()
    {
        assert(lockCount > 0);
        --lockCount;
    }

    Locker getLocker()
    {
        return Locker(this);
    }

    uint32_t curLockCount() const
    {
        return lockCount;
    }

};