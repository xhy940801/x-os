#pragma once

#include <cstdint>

class TaskInfo;

class TaskManagerHelper
{
public:
    static TaskInfo* makeTaskAndSetRet(uintptr_t esp, uintptr_t ebp, int& ret, uintptr_t retPos);
};