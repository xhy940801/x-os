#pragma once

#include <cstdint>

#include "map.h"
#include "task/CpuState.h"
#include "auth.h"


class TaskInfo : MapNode<0>
{
    CpuState cpuState;
    uintptr_t esp;
    uint32_t* catalog;

    AuthInfo auth;

    friend class TaskManager;

    TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, pid_t _pid);

public:
    TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, pid_t _pid, unsigned long options, const TaskInfo& taskInfo);

    TaskInfo() = delete;
    TaskInfo(const TaskInfo&) = delete;
    TaskInfo(TaskInfo&&) = delete;
    TaskInfo& operator = (const TaskInfo&) = delete;
    TaskInfo& operator = (TaskInfo&&) = delete;
};
