#include "wait/WaitInfo.h"
#include "wait.h"

#include "task.h"
#include "sched.h"

void WaitInfo::wait()
{
    waitManager.wait(this);
    scheduleManager.takeOutQueue(static_cast<TaskInfo*>(this), ScheduleInfoState::WAITING);
}

void WaitInfo::sleep(long jiffies)
{
    waitManager.sleep(this, jiffies);
    scheduleManager.takeOutQueue(static_cast<TaskInfo*>(this), ScheduleInfoState::WAITING);
}

void WaitInfo::wakeup(WakeupRet _ret)
{
    ListNode<WaitInfo, 0>::removeSelf();
    ret = _ret;
    scheduleManager.putInQueue(static_cast<TaskInfo*>(this));
}