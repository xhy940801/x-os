#include "multiproc/Cond.h"

#include "multiproc/MultiProcInfo.h"

#include "task.h"
#include "sched.h"

WakeupRet Cond::wait()
{
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());

    list.pushBack(*info);

    info->wait();
    scheduleManager.schedule();

    WakeupRet ret = info->wakeupRet();
    if (ret != WakeupRet::NORMAL)
    {
        info->MultiProcInfo::removeSelf();
        return ret;
    }

    return ret;
}

WakeupRet Cond::wait(long timeout)
{
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());

    list.pushBack(*info);

    info->sleep(timeout);
    scheduleManager.schedule();
    
    WakeupRet ret = info->wakeupRet();
    if (ret != WakeupRet::NORMAL)
    {
        info->MultiProcInfo::removeSelf();
        return ret;
    }

    return ret;
}

void Cond::signal()
{
    auto locker(taskManager.curtask()->getLocker());

    if (list.empty())
        return;

    TaskInfo* next = static_cast<TaskInfo*>(&list.begin().val());
    next->wakeup(WakeupRet::NORMAL);
    next->MultiProcInfo::removeSelf();
}

void Cond::broadcast()
{
    auto locker(taskManager.curtask()->getLocker());

    while (!list.empty())
    {
        TaskInfo* next = static_cast<TaskInfo*>(&list.begin().val());
        next->wakeup(WakeupRet::NORMAL);
        next->MultiProcInfo::removeSelf();
    }
}