#include "wait/WaitManager.h"
#include "wait/WakeupRet.h"

#include "sched.h"
#include "common.h"

void WaitManager::sleep(WaitInfo* info, long jiffies)
{
    JiffiesSegmentation curJiffies(scheduleManager.curJiffies());
    JiffiesSegmentation wakeupJiffies(jiffies + curJiffies.jiffies);

    assert(wakeupJiffies.jiffies != curJiffies.jiffies);

    //should be inserted to wheels[wait::WHEEL_SIZE - 1]
    if (jiffies < 0)
    {
        sleepWheels[wait::WHEEL_SIZE - 1].push(info, wakeupJiffies.segmentation[wait::WHEEL_SIZE - 1]);
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

    sleepWheels[i].push(info, wakeupJiffies.segmentation[i]);
}

void WaitManager::moveWheel(size_t level, JiffiesSegmentation curJiffies)
{
    assert(level > 0);
    if (level < wait::WHEEL_SIZE)
    {
        if (curJiffies.segmentation[level] == 0)
            moveWheel(level + 1, curJiffies);
        WaitInfo* info = sleepWheels[level].pop(curJiffies.segmentation[level]);
        while (info != nullptr)
        {
            sleepWheels[level - 1].push(info, curJiffies.segmentation[level - 1]);
            info = sleepWheels[level].pop(curJiffies.segmentation[level]);
        }
    }
}

void WaitManager::wakeup()
{
    JiffiesSegmentation curJiffies(scheduleManager.curJiffies());

    if (curJiffies.segmentation[0] == 0)
        moveWheel(1, curJiffies);

    WaitInfo* info = sleepWheels[0].begin(curJiffies.segmentation[0]);
    while (info != nullptr)
    {
        info->wakeup(WakeupRet::TIMEOUT);
        info = sleepWheels[0].begin(curJiffies.segmentation[0]);
    }
}