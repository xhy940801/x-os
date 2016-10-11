#include "task/TaskManagerHelper.h"

#include <new>

#include "task.h"
#include "mem.h"
#include "common.h"

TaskInfo* TaskManagerHelper::makeTaskAndSetRet(uintptr_t esp, uintptr_t ebp, int& ret, uintptr_t retPos)
{
    TaskInfo* current = taskManager.curtask();
    TaskInfo* task = reinterpret_cast<TaskInfo*>(memoryManager.allocPages(1));   //alloc 2 page
    if (task == nullptr)
        return nullptr;
    uintptr_t catalogTableP;
    uint32_t* catalog = reinterpret_cast<uint32_t*>(memoryManager.allocOnePage(catalogTableP));
    if (catalog == nullptr)
    {
        memoryManager.freePages(task, 1);
        return nullptr;
    }
    new (task) TaskInfo(catalog, catalogTableP, 0, *current);

    ptrdiff_t stackSize = reinterpret_cast<uintptr_t>(current) + 2 * mem::PAGESIZE - esp;
    task->esp = reinterpret_cast<uintptr_t>(task) + 2 * mem::PAGESIZE - stackSize;

    ptrdiff_t frameSize = reinterpret_cast<uintptr_t>(current) + 2 * mem::PAGESIZE - ebp;
    task->ebp = reinterpret_cast<uintptr_t>(task) + 2 * mem::PAGESIZE - frameSize;

    memcpy(reinterpret_cast<char*>(task->esp), reinterpret_cast<char*>(esp), stackSize);
    task->esp -= 4;
    *reinterpret_cast<uintptr_t*>(task->esp) = retPos;
    ret = task->processID();
    return task;
}