#include "task/TaskManager.h"

#include <new>
#include <utility>

#include "mem.h"
#include "asm.h"
#include "common.h"

extern "C"
{
    void scheduleesp(uintptr_t dst, uintptr_t* src);
    void _setRet(int* ret);
}

char process1page[2 * mem::PAGESIZE];

TaskManager::TaskManager() : current(nullptr)
{
    task0 = reinterpret_cast<TaskInfo*>(process1page);
    new (task0) TaskInfo((uint32_t*) 0xc0100000, 0x00200000, 1);
    current = task0;
}

inline void TaskManager::reloadCpuState()
{
    _frstor(current->cpuState.fpuState);
    _lcr3(current->cpuState.catalogTableP);
}

inline void TaskManager::storeCpuState()
{
    _fnsave(current->cpuState.fpuState);
}

void TaskManager::scheduleto(TaskInfo* target)
{
    assert(target != current);
    storeCpuState();
    std::swap(current, target);
    scheduleesp(current->esp, &target->esp);
    _memorybar();
    reloadCpuState();
}

#pragma GCC push_options
#pragma GCC optimize ("O0", "omit-frame-pointer")

int TaskManager::fork(unsigned long options)
{
    TaskInfo* task = reinterpret_cast<TaskInfo*>(memoryManager.allocPages(1));   //alloc 2 page
    if (task == nullptr)
        return -1;
    uintptr_t catalogTableP;
    uint32_t* catalog = reinterpret_cast<uint32_t*>(memoryManager.allocOnePage(catalogTableP));
    if (catalog == nullptr)
    {
        memoryManager.freePages(task, 1);
        return -1;
    }
    new (task) TaskInfo(catalog, catalogTableP, 1, 0, *current);
    
    int ret = 0;
    uintptr_t esp;
    ptrdiff_t stackSize;
    _lesp(esp);
    stackSize = reinterpret_cast<uintptr_t>(current) + 2 * mem::PAGESIZE - esp;
    task->esp = reinterpret_cast<uintptr_t>(task) + 2 * mem::PAGESIZE - stackSize;
    printk("target %u src %u size %u retpos %u\n", task->esp, esp, stackSize, &&schedule_ret_label);
    memcpy(reinterpret_cast<char*>(task->esp), reinterpret_cast<char*>(esp), stackSize);
    task->esp -= 4;
    *reinterpret_cast<uintptr_t*>(task->esp) = reinterpret_cast<uintptr_t>(&&schedule_ret_label);
    _setRet(&ret);
    _memorybar();
    scheduleto(task);
    _memorybar();
    if (ret != 0)
        return ret;
schedule_ret_label:
    reloadCpuState();
    return 0;
}

#pragma GCC pop_options
