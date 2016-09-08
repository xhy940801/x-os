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

extern "C" {
    int main1();
}

void doParent()
{
    taskManager.curtask()->unlock();
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
    printk("lockcount %u\n", taskManager.curtask()->curLockCount());
    int ret = taskManager.fork(0);
    if (ret != 0)
        doParent();
    else
        doSub();
    return systemParams.memsize();
}
