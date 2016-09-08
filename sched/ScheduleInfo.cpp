#include "sched/ScheduleInfo.h"

#include "sched.h"

#include "common.h"

ScheduleInfo::ScheduleInfo(uint8_t _nice)
    : nice(_nice * 16 + 8), originalNice(_nice), state(ScheduleInfoState::RUNNING), restTime(1), scheduleLoop(0), lockCount(1)
{
    assert(_nice < sched::LEVEL_SIZE);
    scheduleManager.putInQueue(this);
}

ScheduleInfo::ScheduleInfo(unsigned long options, const ScheduleInfo& scheduleInfo)
    : nice(scheduleInfo.nice), originalNice(scheduleInfo.originalNice), state(ScheduleInfoState::RUNNING),
    restTime((scheduleInfo.restTime + 1) / 2), scheduleLoop(scheduleInfo.scheduleLoop), lockCount(scheduleInfo.lockCount)
{
    assert(scheduleInfo.state == ScheduleInfoState::RUNNING);
    scheduleInfo.restTime -= restTime;
    if (scheduleInfo.restTime == 0)
        scheduleInfo.restTime = 1;
    scheduleManager.putInQueue(this);
}