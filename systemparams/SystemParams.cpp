#include "SystemParams.h"

SystemParams systemParams;

extern char _bios_sys_params[];

void initSystemParamsModule()
{
    systemParams.initWithPtr(_bios_sys_params);
}

void SystemParams::initWithPtr(void* ptr)
{
    uint16_t* memparam = (uint16_t*) ptr;
    _memsize = 1024*1024 + memparam[0] * 1024 + memparam[1] * 64 * 1024;
}
