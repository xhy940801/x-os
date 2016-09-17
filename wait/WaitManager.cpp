#include "wait/WaitManager.h"
#include "wait/WakeupRet.h"

#include "task.h"

#include "sched.h"
#include "common.h"

void WaitManager::wait(WaitInfo* info)
{
    static_cast<TaskInfo*>(info)->changeStatus(TaskStatus::WAITING, waitList);
}

void WaitManager::sleep(WaitInfo* info, long jiffies)
{
    JiffiesSegmentation curJiffies(scheduleManager.curJiffies());
    info->wakeupJiffies = jiffies + curJiffies.jiffies;
    JiffiesSegmentation wakeupJiffies(info->wakeupJiffies);

    assert(wakeupJiffies.jiffies != curJiffies.jiffies);

    //should be inserted to wheels[wait::WHEEL_SIZE - 1]
    if (jiffies < 0)
    {
        TaskStatusList& list = sleepWheels[wait::WHEEL_SIZE - 1].dial(wakeupJiffies.segmentation[wait::WHEEL_SIZE - 1]);
        static_cast<TaskInfo*>(info)->changeStatus(TaskStatus::WAITING, list);
        return;
    }

    size_t i;
    for (i = wait::WHEEL_SIZE; i > 0; --i)
    {
        if (wakeupJiffies.segmentation[i - 1] != curJiffies.segmentation[i - 1])
        {
            --i;
            break;
        }
    }

    TaskStatusList& list = sleepWheels[i].dial(wakeupJiffies.segmentation[i]);
    static_cast<TaskInfo*>(info)->changeStatus(TaskStatus::WAITING, list);
}

void WaitManager::moveWheel(size_t level, JiffiesSegmentation curJiffies)
{
    assert(level > 0);
    if (level < wait::WHEEL_SIZE)
    {
        if (curJiffies.segmentation[level] == 0)
            moveWheel(level + 1, curJiffies);
        WaitInfo* info = static_cast<TaskInfo*>(sleepWheels[level].pop(curJiffies.segmentation[level]));
        while (info != nullptr)
        {
            JiffiesSegmentation infoJiffies(info->wakeupJiffies);
            sleepWheels[level - 1].push(static_cast<TaskInfo*>(info), infoJiffies.segmentation[level - 1]);
            info = static_cast<TaskInfo*>(sleepWheels[level].pop(curJiffies.segmentation[level]));
        }
    }
}

void WaitManager::wakeup()
{
    JiffiesSegmentation curJiffies(scheduleManager.curJiffies());

    if (curJiffies.segmentation[0] == 0)
        moveWheel(1, curJiffies);

    WaitInfo* info = static_cast<TaskInfo*>(sleepWheels[0].begin(curJiffies.segmentation[0]));
    while (info != nullptr)
    {
        info->wakeup(WakeupRet::TIMEOUT);
        info = static_cast<TaskInfo*>(sleepWheels[0].begin(curJiffies.segmentation[0]));
    }
}