#include "multiproc/Semaphore.h"

#include "multiproc/MultiProcInfo.h"

#include "common.h"
#include "task.h"
#include "sched.h"

bool Semaphore::tryDown(size_t demand)
{
    assert(demand > 0);
    TaskInfo* info = taskManager.curtask();
    auto locker(info->getLocker());
    if (resource >= demand)
    {
        resource -= demand;
        return true;
    }
    return false;
}

WakeupRet Semaphore::down(size_t demand)
{
    assert(demand > 0);
    TaskInfo* info = taskManager.curtask();
    assert(info->MultiProcInfo::lonely());
    auto locker(info->getLocker());
    if (resource >= demand)
    {
        resource -= demand;
        assert(list.empty());
        return WakeupRet::NORMAL;
    }

    list.pushBack(*info);
    info->demand = demand;
    info->wait();
    scheduleManager.schedule();

    WakeupRet ret = info->wakeupRet();
    if (ret != WakeupRet::NORMAL)
    {
        info->MultiProcInfo::removeSelf();
        return ret;
    }

    assert(resource >= demand);
    resource -= demand;

    return ret;
}

WakeupRet Semaphore::down(size_t demand, long timeout)
{
    assert(demand > 0);
    TaskInfo* info = taskManager.curtask();
    assert(info->MultiProcInfo::lonely());
    auto locker(info->getLocker());
    if (resource >= demand)
    {
        resource -= demand;
        assert(list.empty());
        return WakeupRet::NORMAL;
    }

    list.pushBack(*info);
    info->demand = demand;
    info->sleep(timeout);
    scheduleManager.schedule();

    WakeupRet ret = info->wakeupRet();
    if (ret != WakeupRet::NORMAL)
    {
        info->MultiProcInfo::removeSelf();
        return ret;
    }

    assert(resource >= demand);
    resource -= demand;

    return ret;
}

void Semaphore::up(size_t num)
{
    auto locker(taskManager.curtask()->getLocker());

    assert(num > 0);
    resource += num;
    size_t allDemand = 0;
    while (!list.empty())
    {
        auto it = list.begin();
        allDemand += it->demand;
        if (resource < allDemand)
            break;
        TaskInfo* next = static_cast<TaskInfo*>(&it.val());
        next->wakeup(WakeupRet::NORMAL);
        next->MultiProcInfo::removeSelf();
    }
}