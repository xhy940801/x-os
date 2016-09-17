#include "task/TaskStatus.h"

#include "task/TaskInfo.h"

#include "sched.h"

TaskStatusInfo::TaskStatusInfo()
    : status(TaskStatus::RUNNING), lockCount(1)
{
    TaskStatusList list;
    list.pushBack(*this);
    scheduleManager.putInQueue(static_cast<TaskInfo*>(this));
}