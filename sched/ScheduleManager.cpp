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
    ScheduleInfo* current = taskManager.curtask();
    if (passive)
        current->decNice();
    else
        current->incNice();

    if (current->restTime == 0 && current->state == ScheduleInfoState::RUNNING)
    {
        current->outQueue();
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