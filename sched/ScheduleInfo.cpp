#include "sched/ScheduleInfo.h"

#include "sched.h"

#include "task.h"

#include "common.h"

ScheduleInfo::ScheduleInfo(uint8_t _nice)
    : nice(_nice * 16 + 8), originalNice(_nice), restTime(1), scheduleLoop(0)
{
    assert(_nice < sched::LEVEL_SIZE);
}

ScheduleInfo::ScheduleInfo(unsigned long options, const ScheduleInfo& scheduleInfo)
    : nice(scheduleInfo.nice), originalNice(scheduleInfo.originalNice),
    restTime((scheduleInfo.restTime + 1) / 2), scheduleLoop(scheduleInfo.scheduleLoop)
{
    assert(static_cast<TaskInfo*>(this)->equal(TaskStatus::RUNNING));
    scheduleInfo.restTime -= restTime;
    if (scheduleInfo.restTime == 0)
        scheduleInfo.restTime = 1;
    scheduleManager.putInQueue(this);
}
