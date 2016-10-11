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

    info->MultiProcInfo::removeSelf();
    WakeupRet ret = info->wakeupRet();
    if (ret != WakeupRet::NORMAL)
        return ret;

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

    info->MultiProcInfo::removeSelf();
    WakeupRet ret = info->wakeupRet();
    if (ret != WakeupRet::NORMAL)
        return ret;

    assert(resource >= demand);
    resource -= demand;

    return ret;
}

void Semaphore::up(size_t num)
{
    assert(num > 0);
    resource += num;
    size_t allDemand = 0;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        allDemand += it->demand;
        if (resource < allDemand)
            break;
        static_cast<TaskInfo*>(&it.val())->wakeup(WakeupRet::NORMAL);
    }
}