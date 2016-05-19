#include <SystemParams.h>
#include <vfs.h>
#include <tty0.h>
#include <mem.h>


extern "C" {
    int main1();
}

char process1page[8192]; 

int main1()
{
    initSystemParamsModule();
    initVfsModule();
    initTTY0Module();
    printk("\f");
    initMemModule();
    return systemParams.memsize();
}
