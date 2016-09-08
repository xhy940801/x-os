#pragma once

#include <cstdint>

#include "map.h"
#include "task/CpuState.h"

#include "auth.h"
#include "sched.h"
#include "wait.h"


class TaskInfo : public MapNode<0>, public ScheduleInfo, public AuthInfo, public WaitInfo
{
    CpuState cpuState;
    uintptr_t esp;
    uint32_t* catalog;

    friend class TaskManager;
    friend class TaskManagerHelper;

    TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP);

public:
    TaskInfo(uint32_t* _catalog, uintptr_t _catalogTableP, unsigned long options, const TaskInfo& taskInfo);

    TaskInfo() = delete;
    TaskInfo(const TaskInfo&) = delete;
    TaskInfo(TaskInfo&&) = delete;
    TaskInfo& operator = (const TaskInfo&) = delete;
    TaskInfo& operator = (TaskInfo&&) = delete;
};
