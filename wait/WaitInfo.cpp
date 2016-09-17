#include "wait/WaitInfo.h"
#include "wait.h"

#include "task.h"
#include "sched.h"

void WaitInfo::wait()
{
    waitManager.wait(this);
}

void WaitInfo::sleep(long jiffies)
{
    waitManager.sleep(this, jiffies);
}

void WaitInfo::wakeup(WakeupRet _ret)
{
    ret = _ret;
    scheduleManager.putInQueue(static_cast<TaskInfo*>(this));
}