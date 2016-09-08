#include "sched/ScheduleQueue.h"


#include "sched/ScheduleInfo.h"

#include "common.h"

ScheduleInfo* ScheduleQueue::pop()
{
    for (size_t i = sched::LEVEL_SIZE - 1; i < sched::LEVEL_SIZE; --i)
    {
        if (!empty(i))
        {
            ListNode<ScheduleInfo, 0>* node = levels[i].begin();
            node->removeSelf();
            return static_cast<ScheduleInfo*>(node);
        }
    }
    return nullptr;
}

void ScheduleQueue::push(ScheduleInfo* info)
{
    size_t nice = info->curnice();
    assert(nice < sched::LEVEL_SIZE);
    levels[nice].pushBack(*info);
}