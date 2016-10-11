#include "multiproc/Mutex.h"

#include "multiproc/MultiProcInfo.h"

#include "task.h"
#include "wait.h"
#include "sched.h"

bool Mutex::tryLock()
{
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());

    if(!list.empty())
        return false;

    list.pushBack(*info);
    return true;
}

WakeupRet Mutex::lock()
{
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());

    bool empty = list.empty();
    list.pushBack(*info);

    if (empty)
        return WakeupRet::NORMAL;

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

WakeupRet Mutex::lock(long timeout)
{
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());

    bool empty = list.empty();
    list.pushBack(*info);

    if (empty)
        return WakeupRet::NORMAL;

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

void Mutex::unlock()
{
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());

    assert(&list.begin().val() == info);
    info->MultiProcInfo::removeSelf();

    if (list.begin() != list.end())
    {
        TaskInfo* next = static_cast<TaskInfo*>(&list.begin().val());
        next->wakeup(WakeupRet::NORMAL);
    }
}