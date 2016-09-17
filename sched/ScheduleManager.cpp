#include "sched/ScheduleManager.h"

#include <utility>

#include "task.h"
#include "common.h"

ScheduleManager::ScheduleManager()
    : scheduleLoop(0), jiffies(0)
{
    active = queue + 0;
    expire = queue + 1;
}

void ScheduleManager::schedule(bool passive)
{
    TaskInfo* current = taskManager.curtask();
    if (passive)
        current->decNice();
    else
        current->incNice();

    if (current->restTime == 0 && current->equal(TaskStatus::RUNNING))
    {
        current->TaskStatusInfo::removeSelf();
        current->resetRestTime();
        expire->push(current);
    }

    ScheduleInfo* target = active->pop();

    if (target == nullptr)
    {
        std::swap(active, expire);
        ++scheduleLoop;
        target = active->pop();
    }

    assert(target != nullptr);
    assert(target->restTime > 0);
    --target->restTime;
    active->push(target);

    if (target != current)
        taskManager.switchTask(static_cast<TaskInfo*>(target));
}

void ScheduleManager::putInQueue(ScheduleInfo* info)
{
    TaskStatusList* list;
    if (info->restTime == 0)
    {
        info->resetRestTime();
        if (info->scheduleLoop == scheduleLoop)
            list = &expire->level(info);
        else
            list = &active->level(info);
    }
    else
        list = &active->level(info);
    static_cast<TaskInfo*>(info)->changeStatus(TaskStatus::RUNNING, *list);
}