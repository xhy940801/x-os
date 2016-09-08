#pragma once

#include "task/TaskInfo.h"

class TaskManager
{
    TaskInfo* task0;
    TaskInfo* current;

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
