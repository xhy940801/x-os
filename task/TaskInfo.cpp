#include "task/TaskInfo.h"

#include "common.h"
#include "mem.h"

TaskInfo::TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, pid_t _pid)
    : cpuState(_catalogTableP), catalog(_catalog), auth(_pid)
{
}

TaskInfo::TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, pid_t _pid, unsigned long options, const TaskInfo& taskInfo)
    : cpuState(_catalogTableP, options, taskInfo.cpuState), catalog(_catalog), auth(_pid, options, taskInfo.auth)
{
    memcpy(catalog, taskInfo.catalog, mem::PAGESIZE);
}