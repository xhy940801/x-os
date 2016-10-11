#pragma once

#include <cstdint>

#include "map.h"
#include "task/CpuState.h"
#include "task/TaskStatus.h"

#include "auth/AuthInfo.h"
#include "sched/ScheduleInfo.h"
#include "wait/WaitInfo.h"
#include "multiproc/MultiProcInfo.h"
#include "errno/ErrnoInfo.h"

class TaskInfo : public MapNode<0>, public TaskStatusInfo, public ScheduleInfo, public AuthInfo, public WaitInfo, public MultiProcInfo, public ErrnoInfo
{
    CpuState cpuState;
    uintptr_t esp;
    uintptr_t ebp;
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

public:
    struct Less
    {
        bool operator () (const TaskInfo& t1, const TaskInfo& t2)
        {
            return t1.processID() < t2.processID();
        }
    };
};
