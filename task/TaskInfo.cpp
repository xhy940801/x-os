#include "task/TaskInfo.h"

#include "common.h"
#include "mem.h"

TaskInfo::TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP)
    : ScheduleInfo(0), AuthInfo(), WaitInfo(), cpuState(_catalogTableP), catalog(_catalog)
{
}

TaskInfo::TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, unsigned long options, const TaskInfo& taskInfo)
    : ScheduleInfo(options, taskInfo), AuthInfo(options, taskInfo), WaitInfo(options, taskInfo),
    cpuState(_catalogTableP, options, taskInfo.cpuState), catalog(_catalog)
{
    memcpy(catalog, taskInfo.catalog, mem::PAGESIZE);
}