#include "harddiskdriver.h"

#include <new>

#include "harddiskdriver/RealHardDiskDriver.h"

#include "interrupt.h"
#include "task.h"

RealHardDiskDriver hddriver0(0, 0);

extern "C"
{
    void on_hd_interrupt();
    void doHDInterrupt();
}

void initHardDiskDriverModule()
{
    new (&hddriver0) RealHardDiskDriver(0x01f0, 0);

    clear8259Mask(14);
    setupIntrDesc(0x2e, reinterpret_cast<void*>(on_hd_interrupt), 0);
}

void doHDInterrupt()
{
    TaskInfo* current = taskManager.curtask();
    current->vLock();

    _outb(0xa0, 0x20);
    _outb(0x20, 0x20);

    hddriver0.doIRQ();    

    current->vUnlock();
}

HardDiskDriver* getHD(size_t i)
{
    return hddriver0.getPartition(i);
}