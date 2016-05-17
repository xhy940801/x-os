#include <SystemParams.h>
#include <vfs.h>
#include <tty0.h>


extern "C" {
    int main1();
}

char process1page[8192]; 

int main1()
{
    initSystemParamsModule();
    initVfsModule();
    initTTY0Module();
    const char* str = "\fhaha";
    tty0vfs.getRootInode(DriverType(0, MainDriverType::TTY0))->write(reinterpret_cast<const uint8_t*>(str), strlen(str), nullptr);
    return systemParams.memsize();
}
