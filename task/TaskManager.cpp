#include "task/TaskManager.h"

#include <new>
#include <utility>

#include "mem.h"
#include "asm.h"
#include "common.h"
#include "task/TaskManagerHelper.h"
#include "task/CpuDescriptor.h"

extern "C"
{
    void _task_swapesp(uintptr_t esp_dst, uintptr_t* esp_src, uintptr_t ebp_dst, uintptr_t* ebp_src);
}

char process1page[2 * mem::PAGESIZE];
tss_struct cpu0tss;
extern gdt_descriptor _gdt[];

TaskManager::TaskManager() : current(nullptr)
{
    task0 = reinterpret_cast<TaskInfo*>(process1page);
    new (task0) TaskInfo((uint32_t*) 0xc0100000, 0x00200000);
    current = task0;

    cpu0tss.ss0 = 0x10;
    cpu0tss.esp0 = reinterpret_cast<uint32_t>(current) + 2 * mem::PAGESIZE;

    gdt_descriptor* pdt = _gdt + 3;
    pdt->l_limit = (short) (sizeof(tss_struct));
    pdt->l_base = (short) (((unsigned int) &cpu0tss) & 0xffff);
    pdt->m_base = (char) ((((unsigned int) &cpu0tss) >> 16) & 0xff);
    pdt->attr = 0x89 | ((sizeof(tss_struct) >> 8) & 0x0f00);
    pdt->h_base = (char) ((((unsigned int) &cpu0tss) >> 24) & 0xff);

    _ltr(3 * 8);
}

inline void TaskManager::reloadCpuState()
{
    _frstor(current->cpuState.fpuState);
    _lcr3(current->cpuState.catalogTableP);
    cpu0tss.esp0 = reinterpret_cast<uint32_t>(current) + 2 * mem::PAGESIZE;
}

inline void TaskManager::storeCpuState()
{
    _fnsave(current->cpuState.fpuState);
}

void TaskManager::switchTask(TaskInfo* target)
{
    assert(target != current);
    storeCpuState();
    std::swap(current, target);
    _task_swapesp(current->esp, &target->esp, current->ebp, &target->ebp);
    _memorybar();
    reloadCpuState();
}

#pragma GCC push_options
#pragma GCC optimize ("O0", "omit-frame-pointer")

int TaskManager::fork(unsigned long options)
{
    int ret = 0;
    uintptr_t esp;
    _lesp(esp);
    TaskInfo* task = TaskManagerHelper::makeTaskAndSetRet(esp, ret, reinterpret_cast<uintptr_t>(&&schedule_ret_label));
    switchTask(task);
    _memorybar();
    if (ret != 0)
        return ret;
schedule_ret_label:
    reloadCpuState();
    return 0;
}

#pragma GCC pop_options