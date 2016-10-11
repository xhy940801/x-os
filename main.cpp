#include "SystemParams.h"
#include "vfs.h"
#include "tty0.h"
#include "mem.h"
#include "task.h"
#include "sched.h"
#include "auth.h"
#include "wait.h"
#include "common.h"
#include "asm.h"
#include "pci.h"
#include "harddiskdriver.h"
#include "interrupt.h"

extern "C" {
    int main1();
}

void doParent()
{
    taskManager.curtask()->unlock();
    BlockDriver* driver = getHD(1);
    char* data = static_cast<char*>(memoryManager.allocPages(0));
    char* data2 = static_cast<char*>(memoryManager.allocPages(0));
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;

    data2[0] = 21;
    data2[1] = 22;
    data2[2] = 23;
    char* datas[2] = {data, data2};
    int rs = driver->write(0, datas, 2);
    printk("finish rs %d\n", rs);
    data[0] = 100;
    rs = driver->write(0, &data, 1);
    printk("finish rs %d %u %u %u\n", rs, data[0], data[1], data[2]);
    while(1);
}

void doSub()
{
    taskManager.curtask()->unlock();
    while(1);
}

int main1()
{
    initSystemParamsModule();
    memset(reinterpret_cast<uint32_t*>(mem::KMEMSTART + 0x100000), 0, 3 * 1024);
    _lcr3(0x200000);

    initVfsModule();
    initTTY0Module();
    printk("\f");

    initMemModule();
    initSchedModule();
    initAuthModule();
    initWaitModule();
    initTaskModule();
    memoryManager.resetSemaphore();
    printk("lockcount %u\n", taskManager.curtask()->curLockCount());

    initPCIModule();
    initHardDiskDriverModule();

    clear8259Mask(2);

    int ret = taskManager.fork(0);
    if (ret != 0)
        doParent();
    else
        doSub();
    return systemParams.memsize();
}
