#include <SystemParams.h>


extern "C" {
    int main1();
}

char process1page[8192]; 

int main1()
{
    initSystemParamsModule();
    return systemParams.memsize();
}
