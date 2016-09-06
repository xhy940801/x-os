#include "SystemParams.h"
#include "vfs.h"
#include "tty0.h"
#include "mem.h"
#include "task.h"
#include "common.h"
#include "asm.h"

extern "C" {
    int main1();
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
    initTaskModule();
    printk("cur %u\n", taskManager.curtask());
    int ret = taskManager.fork(0);
    printk("ret %d cur %u\n", ret, taskManager.curtask());
    if (taskManager.curtask() != taskManager.process0task())
        taskManager.scheduleto(taskManager.process0task());
    printk("cur %u\n", taskManager.curtask());
    return systemParams.memsize();
}
