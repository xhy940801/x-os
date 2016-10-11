#include "sched/ScheduleQueue.h"


#include "sched/ScheduleInfo.h"
#include "task.h"

#include "common.h"

ScheduleInfo* ScheduleQueue::pop()
{
    for (size_t i = sched::LEVEL_SIZE - 1; i < sched::LEVEL_SIZE; --i)
    {
        if (!empty(i))
        {
            TaskInfo* node = static_cast<TaskInfo*>(&levels[i].begin().val());
            node->TaskStatusInfo::removeSelf();
            return node;
        }
    }
    return nullptr;
}

void ScheduleQueue::push(ScheduleInfo* info)
{
    size_t nice = info->curnice();
    assert(nice < sched::LEVEL_SIZE);
    levels[nice].pushBack(*static_cast<TaskInfo*>(info));
}

TaskStatusList& ScheduleQueue::level(ScheduleInfo* info)
{
    size_t nice = info->curnice();
    assert(nice < sched::LEVEL_SIZE);
    return levels[info->nice];
}