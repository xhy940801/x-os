#include "task/TaskInfo.h"

#include "task.h"

#include "common.h"
#include "mem.h"

TaskInfo::TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP)
    : TaskStatusInfo(), ScheduleInfo(0), AuthInfo(), WaitInfo(), MultiProcInfo(), ErrnoInfo(), cpuState(_catalogTableP), catalog(_catalog)
{
    taskManager.add(*this);
}

TaskInfo::TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, unsigned long options, const TaskInfo& taskInfo)
    : TaskStatusInfo(options, taskInfo), ScheduleInfo(options, taskInfo),
    AuthInfo(options, taskInfo), WaitInfo(options, taskInfo), MultiProcInfo(options, taskInfo), ErrnoInfo(options, taskInfo),
    cpuState(_catalogTableP, options, taskInfo.cpuState), catalog(_catalog)
{
    memcpy(catalog, taskInfo.catalog, mem::PAGESIZE);
    taskManager.add(*this);
}