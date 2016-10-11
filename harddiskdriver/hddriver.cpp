#include "harddiskdriver.h"

#include <new>

#include "harddiskdriver/RealHardDiskDriver.h"

#include "interrupt.h"

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
    hddriver0.doIRQ();
}

HardDiskDriver* getHD(size_t i)
{
    return hddriver0.getPartition(i);
}