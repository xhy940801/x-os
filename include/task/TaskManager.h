#pragma once

#include "task/TaskInfo.h"

#include "Map.h"

class TaskManager
{
    TaskInfo* task0;
    TaskInfo* current;

    Map<TaskInfo, 0, TaskInfo::Less> taskMap;

    void add(TaskInfo& taskInfo)
    {
        taskMap.insert(&taskInfo);
    }

    void remove(TaskInfo& taskInfo)
    {
        taskMap.remove(&taskInfo);
    }

    friend class TaskInfo;

public:
    TaskManager();

    void switchTask(TaskInfo* target);

    void reloadCpuState();
    void storeCpuState();

    int fork(unsigned long options);

public:
    TaskInfo* curtask()
    {
        return current;
    }

    TaskInfo* process0task()
    {
        return task0;
    }
};
