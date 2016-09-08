#include "sched/ScheduleManager.h"

#include "task.h"

#include "asm.h"
#include "interrupt.h"

#include <new>

ScheduleManager scheduleManager;

extern "C"
{
    void doTimer();
    void on_timer_interrupt();
    void on_spurious_interrupt();
}

void initSchedModule()
{
    new (&scheduleManager) ScheduleManager();

    setupIntrDesc(0x20, reinterpret_cast<void*>(on_timer_interrupt), 0);
    setupIntrDesc(0x27, reinterpret_cast<void*>(on_spurious_interrupt), 0);

    const size_t thz = 1193180;
    const size_t od = thz / sched::TIMER_HZ;
    _outb(0x43, 0x36);
    _outb(0x40, od & 0xff);
    _outb(0x40, (od >> 8) & 0xff);

    clear8259Mask(0);
}

void doTimer()
{
    TaskInfo* current = taskManager.curtask();
    current->vLock();

    _outb(0x20, 0x20);

    scheduleManager.addJiffies();

    scheduleManager.schedule(true);

    current->vUnlock();
}